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
#include "fillers.h"

extern std::string interface_name {"enp0s25"};
std::string dst_ip = "169.254.79.181";
macaddr_t dst_mac = {0xb8, 0x27, 0xeb, 0xd4, 0x45, 0x84};

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

sockaddr_ll create_sockaddr(int s){
    struct ifreq if_idx;
    struct sockaddr_ll socket_address;

    /* Get the index of the interface to send on */
    memset(&if_idx, 0, sizeof(struct ifreq));

    if (interface_name.size() >= IFNAMSIZ) {
        fprintf(stderr, "Too long iface name");
        exit(-1);
    }

    strncpy(if_idx.ifr_name, interface_name.c_str(), IFNAMSIZ-1);
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

int main(int argc, char **argv) {
    signal(SIGALRM, sig_alrm);
    sig_alrm(SIGALRM);		/* send first packet */

    char packet[42];
    char* first = packet;

    memset(packet, 'A', sizeof(packet));   // payload will be all As

    int s;
    if((s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        perror("error:");
        exit(EXIT_FAILURE);
    }

    sockaddr_ll socket_address = create_sockaddr(s);

    auto  src_mac = get_mac_from_iface(s, interface_name);
    fill_ethernet(first, dst_mac, src_mac);

    in_addr_t dst, src;
    src = get_ip_from_iface(interface_name);
    inet_pton(AF_INET, dst_ip.c_str(), (struct in_addr *) &dst);
    fill_ip(first, packet, dst, src, sizeof(packet));
    fill_icmp(first, packet, sizeof(packet));


    while (42) {
        sleep(1);
        if (sendto(s, packet, sizeof(packet), 0,
                   (struct sockaddr *) &socket_address, (socklen_t) sizeof(sockaddr_ll)) < 0)
            perror("uh oh:");
    }
    return (0);
}



