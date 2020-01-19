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

std::string interface_name; //{"enx9cd643a36df7"};
std::string dst_ip = "169.254.79.181";
macaddr_t dst_mac = {0xb8, 0x27, 0xeb, 0xd4, 0x45, 0x84};
//macaddr_t dst_mac = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

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

sockaddr_ll create_sockaddr_for_send(int s) {
    struct ifreq if_idx;
    struct sockaddr_ll socket_address;

    /* Get the index of the interface to send on */
    memset(&if_idx, 0, sizeof(struct ifreq));

    if (interface_name.size() >= IFNAMSIZ) {
        fprintf(stderr, "Too long iface name");
        exit(-1);
    }

    strncpy(if_idx.ifr_name, interface_name.c_str(), IFNAMSIZ - 1);
    if (ioctl(s, SIOCGIFINDEX, &if_idx) < 0)
        perror("SIOCGIFINDEX");

    /* Index of the network device */
    socket_address.sll_ifindex = if_idx.ifr_ifindex;
    /* Address length*/
    socket_address.sll_halen = ETH_ALEN;
    /* Destination MAC */
    memcpy(socket_address.sll_addr, &dst_mac[0], IFHWADDRLEN);
    return socket_address;
}

sockaddr_ll create_sockaddr_for_receive(int s) {
    struct ifreq if_idx;
    struct sockaddr_ll socket_address{0};

    /* Get the index of the interface to send on */
    memset(&if_idx, 0, sizeof(struct ifreq));

    if (interface_name.size() >= IFNAMSIZ) {
        fprintf(stderr, "Too long iface name");
        exit(-1);
    }

    strncpy(if_idx.ifr_name, interface_name.c_str(), IFNAMSIZ - 1);
    if (ioctl(s, SIOCGIFINDEX, &if_idx) < 0)
        perror("SIOCGIFINDEX");


    socket_address.sll_family = AF_PACKET;

    /* Index of the network device */
    socket_address.sll_ifindex = if_idx.ifr_ifindex;
    socket_address.sll_protocol = htons(ETH_P_ALL);

    return socket_address;
}

void recv_loop() {
    int s;
    if ((s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        perror("error:");
        exit(EXIT_FAILURE);
    }
    sockaddr_ll socket_address = create_sockaddr_for_receive(s);

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

                if(0 == memcmp(arp->ar_tha,  &get_mac_from_iface(s, interface_name)[0], IFHWADDRLEN)){
                    memcpy(&dst_mac[0], arp->ar_sha, IFHWADDRLEN); //TODO add work with arp table
                    std::cout<<"dst mac = ";
                    for(auto&& i : dst_mac)
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

int main(int argc, char **argv) {

    uint32_t addr;
    inet_pton(AF_INET, dst_ip.c_str(), (struct in_addr *) &addr);
    auto res = get_routes(addr);

    if(!res.empty()) {
        char if_nam_buf[IF_NAMESIZE];
        interface_name = if_indextoname(res.front()._iface_id, if_nam_buf);
        std::cout<<"interface name: " << interface_name<<std::endl;
    }

    signal(SIGALRM, sig_alrm);
    sig_alrm(SIGALRM);


    char *first = packet;

    memset(packet, 'A', sizeof(packet));   // payload will be all As

    int s;
    if ((s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        perror("error:");
        exit(EXIT_FAILURE);
    }

//    memset(packet, 0, sizeof(packet));
    sockaddr_ll socket_address = create_sockaddr_for_send(s);

    auto src_mac = get_mac_from_iface(s, interface_name);
    //resolve arp
    {
        fill_ethernet(first, dst_mac, src_mac, ETH_P_ARP/*ETH_P_IP*/);

        in_addr_t dst, src;
        src = get_ip_from_iface(interface_name);
        inet_pton(AF_INET, dst_ip.c_str(), (struct in_addr *) &dst);
        fill_arp_request(first, src_mac, dst, src);

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
        fill_ethernet(first, dst_mac, src_mac, ETH_P_IP);

        in_addr_t dst, src;
        src = get_ip_from_iface(interface_name);
        inet_pton(AF_INET, dst_ip.c_str(), (struct in_addr *) &dst);


        fill_ip(first, packet, dst, src, pyaload_size + icmp_header_size);
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



