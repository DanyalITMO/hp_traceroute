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


std::size_t pyaload_size = 200;
void sig_alrm(int signo) {
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

    alarm(1);
    return;
}

char packet[65536];

sockaddr_ll create_sockaddr_for_send() {
    struct sockaddr_ll socket_address;
    socket_address.sll_ifindex = s_config._iface_index;
    socket_address.sll_halen = ETH_ALEN;
    memcpy(socket_address.sll_addr, &s_config._next_hop_mac[0], IFHWADDRLEN); //NOTE may be can delete this line and combine with create_sockaddr_for_receive()
    return socket_address;
}

sockaddr_ll create_sockaddr_for_receive() {
    struct sockaddr_ll socket_address{0};
    socket_address.sll_family = AF_PACKET;
    socket_address.sll_ifindex = s_config._iface_index;
    socket_address.sll_protocol = htons(ETH_P_ALL);

    return socket_address;
}

void recv_loop() {
    int s;
    if ((s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        perror("error:");
        exit(EXIT_FAILURE);
    }
    sockaddr_ll socket_address = create_sockaddr_for_receive();

    while (true) {
//        sleep(1);
        char *first = packet;
//        socklen_t sockaddr_size;
        int saddr_size = sizeof(socket_address);
        if (recvfrom(s, packet, sizeof(packet), 0,
                     (struct sockaddr *) &socket_address, (socklen_t*)&saddr_size) < 0)
            perror("recv_loop:");

        ether_header *eh = (struct ether_header *) first;
        first += sizeof(ether_header);
        switch (htons(eh->ether_type)) {
            case ETH_P_ARP: {
                my_arphdr *arp = reinterpret_cast<my_arphdr *>(first);
                first += sizeof(my_arphdr);

                if(0 == memcmp(arp->ar_tha,  &s_config._iface_mac[0], IFHWADDRLEN)){
                    memcpy(&s_config._next_hop_mac[0], arp->ar_sha, IFHWADDRLEN); //TODO add work with arp table
                    std::cout<<"dst mac = ";
                    for(auto&& i : s_config._next_hop_mac)
                        std::cout << std::hex << (int)i << ":";
                    std::cout<<std::endl;
                    return;
                }
                break;
            }
            case ETH_P_IP:
                break;
            default:
                std::cerr << "!!!!unknown proto"<<std::endl;
        }
//        if( ETH_P_ARP/*ETH_P_IP*/ == eh->ether_type)
    }
}
void init(){
    std::string dst_ip = "169.254.79.181";

    inet_pton(AF_INET, dst_ip.c_str(), (struct in_addr *) &s_config._dst_ip);
    auto res = get_routes(s_config._dst_ip);

    if(!res.empty()) {
        auto&& route = res.front(); // TODO add select path
        s_config._iface_index = route._iface_id;
        char if_nam_buf[IF_NAMESIZE];
        s_config._iface_name = if_indextoname(res.front()._iface_id, if_nam_buf);
//        std::cout<<"interface name: " << s_config._iface_name<<std::endl;
    }

    s_config._iface_ip = get_ip_from_iface(s_config._iface_name);
    s_config._iface_mac = get_mac_from_iface(s_config._iface_name);
    s_config._next_hop_mac = {0xb8, 0x27, 0xeb, 0xd4, 0x45, 0x84};
}

int main(int argc, char **argv) {
    init();

    signal(SIGALRM, sig_alrm);
    sig_alrm(SIGALRM);


    char *first = packet;

    memset(packet, 'A', sizeof(packet));   // payload will be all As

    int s;
    if ((s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        perror("error:");
        exit(EXIT_FAILURE);
    }

    sockaddr_ll socket_address = create_sockaddr_for_send();

    //resolve arp
    {
        fill_ethernet(first, s_config._next_hop_mac, s_config._iface_mac, ETH_P_ARP/*ETH_P_IP*/);
        fill_arp_request(first, s_config._iface_mac, s_config._dst_ip, s_config._iface_ip);

//        while(true) {
            sleep(1);
            if (sendto(s, packet, first - packet, 0,
                       (struct sockaddr *) &socket_address, (socklen_t) sizeof(sockaddr_ll)) < 0)
                perror("main:");
//        }
        recv_loop();
    }
    {
        first = packet;
        fill_ethernet(first, s_config._next_hop_mac, s_config._iface_mac, ETH_P_IP);
        fill_ip(first, packet, s_config._dst_ip, s_config._iface_ip, pyaload_size + icmp_header_size);
        fill_icmp(first, packet, pyaload_size);
        first += pyaload_size;
    }

    while (42) {
//        sleep(1);
        if (sendto(s, packet, first - packet, 0,
                   (struct sockaddr *) &socket_address, (socklen_t) sizeof(sockaddr_ll)) < 0)
            perror("uh oh:");
    }
    return (0);
}



