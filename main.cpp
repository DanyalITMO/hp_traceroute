#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/if.h>
#include <unistd.h>
#include <iostream>
#include <string_view>
#include <csignal>
#include "stats.h"
#include <netinet/in.h>
#include "utils.h"
#include <linux/if_packet.h>
#include <linux/rtnetlink.h>
#include "fillers.h"
#include "route.h"
#include "states.h"
#include "config.h"
#include "handler_ipv4.h"
#include "arp.h"
#include <thread>
#include <atomic>

bool got_alarm = false;

void print_stats(){
      static stats_data previous;
      auto now = get_all_params();
      std::cout << "-------------NIC stats-------------" << "\n";
      std::cout << "dTX packets: " << now.tx_packets - previous.tx_packets << "\n";
      std::cout << "dRX packets: " << now.rx_packets - previous.rx_packets << "\n";
  //        std::cout << "dTX bytes: " << now.tx_bytes - previous.tx_bytes << "\n";
  //        std::cout << "dRX bytes: " << now.rx_bytes - previous.rx_bytes << std::endl;
      {
          auto[n, s] = get_max_measurement((now.tx_bytes - previous.tx_bytes) * 8);
          std::cout << "dTX: " << n << s << "\n";
      }
      {
          auto[n, s] = get_max_measurement((now.rx_bytes - previous.rx_bytes) * 8);
          std::cout << "dRX: " << n << s << "\n";
      }
      previous = now;

}


std::vector<in_addr_t> route;
decltype (route)::iterator current;
void sig_alrm(int signo) {

//    print_stats();
    switch (s_config._state) {
        case STATE::RESOLVE_ARP :
            arp::send_request(s_config._dst_ip);
            alarm(1);
            break;
        case STATE::GET_PATH :
            got_alarm = true;
            alarm(1);
            break;
        case STATE::GENERATE_LOAD : {
            static bool first = true;
            if(first){
                current = route.begin();
                first = false;
            } else {
                current++;
            }
            if(std::end(route) == current)
                s_config._state = STATE::FINISH;
            alarm(1);
            break;
            }
        case STATE::FINISH :

        break;

    }
}
sockaddr_ll create_sockaddr() {
    struct sockaddr_ll socket_address;
    socket_address.sll_ifindex = s_config._iface_index;
    socket_address.sll_halen = ETH_ALEN;
//    memcpy(socket_address.sll_addr, &s_config._next_hop_mac[0], IFHWADDRLEN); //NOTE may be can delete this line and combine with create_sockaddr_for_receive()
    return socket_address;
}

void init() {
//    std::string dst_ip = "169.254.79.181";
    std::string dst_ip = "8.8.8.8";

    inet_pton(AF_INET, dst_ip.c_str(), (struct in_addr *) &s_config._dst_ip);
    auto res = get_routes(s_config._dst_ip);

    if (!res.empty()) {
        auto &&route = res.front(); // TODO add select path
        s_config._iface_index = route._iface_id;
        char if_nam_buf[IF_NAMESIZE];
        s_config._iface_name = if_indextoname(res.front()._iface_id, if_nam_buf);
        if(route._gw)
            s_config._next_hop_ip = *route._gw;
        else
            s_config._next_hop_ip = s_config._dst_ip; // local network

//        char buf[256];
//        std::cerr<<"!!!!:next_hop_ip "<<  inet_ntop(AF_INET, reinterpret_cast<void*>(&s_config._next_hop_ip), buf, sizeof(buf))<<std::endl;

//        std::cout<<"interface name: " << s_config._iface_name<<std::endl;
    }

    s_config._iface_ip = get_ip_from_iface(s_config._iface_name);
    s_config._iface_mac = get_mac_from_iface(s_config._iface_name);
//    s_config._next_hop_mac = {0xb8, 0x27, 0xeb, 0xd4, 0x45, 0x84};
    s_config._payload_size = 200;

    send_socket_addr = create_sockaddr();
    recv_socket_addr = create_sockaddr();

    timeval tv{3, 0};

    setsockopt(recv_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}


std::string convert_speed(uint32_t speed){
    uint32_t kbit = 1024;
    uint32_t mbit = 1024 * kbit;
    uint32_t gbit = 1024 * mbit;

    if(speed > gbit)
        return std::to_string(static_cast<double>(speed)/ gbit) + "gbit/s";
    if(speed > mbit)
        return std::to_string(static_cast<double>(speed)/ mbit) + "mbit/s";
    if(speed > kbit)
        return std::to_string(static_cast<double>(speed)/ kbit) + "kbit/s";
}

int main(int argc, char **argv) {
    char buf[256];

    init();

    signal(SIGALRM, sig_alrm);
    sig_alrm(SIGALRM);

    arp::send_request(s_config._next_hop_ip );

    memset(packet, 'A', sizeof(packet));   // payload will be all As

    macaddr_t empty{0, 0, 0, 0, 0, 0};
    while (0 == memcmp(&empty[0], &s_config._next_hop_mac[0], IFHWADDRLEN)) {
        process_arp();
//        std::cerr<<"resolving arp"<<std::endl;
    }
    s_config._state = STATE::GET_PATH;



    bool end = false;
    for (uint8_t ttl = 1; ttl <= 64 && end == false; ttl++) {
        char *first = packet;
        fill_ethernet(first, s_config._next_hop_mac, s_config._iface_mac, ETH_P_IP);
        fill_ip(first, s_config._dst_ip, s_config._iface_ip, s_config._payload_size + icmp_header_size, ttl);
        fill_icmp(first, s_config._payload_size);
        first += s_config._payload_size;

        if (sendto(send_socket, packet, first - packet, 0,
                   (struct sockaddr *) &send_socket_addr, (socklen_t) sizeof(sockaddr_ll)) < 0)
            perror("uh oh:");

        RET rc{RET::WRONG_ADDR};
        int probe_count = 3;
        while (rc != RET::SUCCESS && probe_count != 0) {
            rc = process_icmp(route, end);
            if(rc == RET::SUCCESS){

                if(!route.empty())
                    std::cerr<<"ROUTE: "<<  inet_ntop(AF_INET, reinterpret_cast<void*>(&route.back()), buf, sizeof(buf));
            }
            if (rc == RET::TIMEOUT || rc == RET::RECV_ERROR) {
                std::cerr << "*";
                probe_count--;
            }
        }
        std::cerr << std::endl;
    }

    s_config._state = STATE::GENERATE_LOAD;
    raise(SIGALRM);

    std::map<in_addr_t, std::size_t> recv_statistc, send_statistic;
    std::atomic<bool> f{true};
    std::thread th{[&f, &recv_statistc](){
            while(f){
                process_icmp_load(recv_statistc);
            }
            }};
    while(STATE::GENERATE_LOAD == s_config._state){
        char *first = packet;
        fill_ethernet(first, s_config._next_hop_mac, s_config._iface_mac, ETH_P_IP);
        fill_ip(first, *current, s_config._iface_ip, s_config._payload_size + icmp_header_size);
        fill_icmp(first, s_config._payload_size);
        first += s_config._payload_size;
        if (sendto(send_socket, packet, first - packet, 0,
                   (struct sockaddr *) &send_socket_addr, (socklen_t) sizeof(sockaddr_ll)) < 0)
            perror("uh oh:");



        if(send_statistic.count(*current)){
            try {
                send_statistic.at(*current)++;

            } catch (...) {
                std::cerr<<send_statistic.count(*current)<<std::endl;
            }
        }
        else
            send_statistic.emplace(*current, 0);
    }


    f = false;
    th.join();

    auto packet_size = (sizeof(ether_header) +  sizeof(iphdr) + icmp_header_size + s_config._payload_size) * 8;
    for(auto&& it : route){
        if(recv_statistc.count(it))
            std::cerr<<inet_ntop(AF_INET, reinterpret_cast<void*>(&it), buf, sizeof(buf))<<"recv: "<< convert_speed(recv_statistc.at(it) *packet_size) <<
                    ", send: "<< convert_speed(send_statistic.at(it) *packet_size) <<std::endl;

        else
            std::cerr<<inet_ntop(AF_INET, reinterpret_cast<void*>(&it), buf, sizeof(buf))<<"- "<< 0 << "bit/s"<<std::endl;
    }

    return (0);
}



