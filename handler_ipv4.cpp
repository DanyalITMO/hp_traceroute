#include <linux/if_packet.h>
#include "handler_ipv4.h"
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/if.h>
#include <iostream>
#include <netinet/in.h>
#include <netinet/udp.h>
#include "utils.h"
#include "fillers.h"
#include "config.h"


RET recv(std::size_t& packet_size, ether_header*& eh) {
//    int rc = 0;
    int s;
    if ((s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        perror("error:");
        exit(EXIT_FAILURE);
    }

    char *first = packet;
    int saddr_size = sizeof(recv_socket_addr);

    int rc = recvfrom(s, packet, sizeof(packet), 0,
                      (struct sockaddr *) &recv_socket_addr, (socklen_t *) &saddr_size) < 0;

    if (rc < 0) {
        packet_size = 0;

        if (EWOULDBLOCK == errno) {
            return RET::TIMEOUT;
        } else {
            perror("recv_loop:");
            return RET::RECV_ERROR;
        }
    } else {
        packet_size = rc;
    }

    eh = (struct ether_header *) first;
//    first += sizeof(ether_header);

    return RET::SUCCESS;
}

RET process_arp() {
    ether_header *eh;
    char *first = packet + sizeof(ether_header);
    std::size_t packet_size;
    auto rc = recv(packet_size, eh);
    if (rc != RET::SUCCESS)
        return rc;

    if (ETH_P_ARP == htons(eh->ether_type)) {
        my_arphdr *arp = reinterpret_cast<my_arphdr *>(first);
        first += sizeof(my_arphdr);

        if (0 != memcmp(arp->ar_tha, &s_config._iface_mac[0], IFHWADDRLEN))
            return RET::WRONG_ADDR;

        memcpy(&s_config._next_hop_mac[0], arp->ar_sha, IFHWADDRLEN); //TODO add work with arp table
        std::cout << "dst mac = ";
        for (auto &&i : s_config._next_hop_mac)
            std::cout << std::hex << (int) i << ":";
        std::cout << std::endl;
    }
    return RET::SUCCESS;
}

RET process_icmp(std::vector<in_addr_t>& route, bool& end) {
    ether_header *eh;
    char *first = packet + sizeof(ether_header);
    std::size_t packet_size;
    auto rc = recv(packet_size, eh);
    if (rc != RET::SUCCESS)
        return rc;

    if (ETH_P_IP == htons(eh->ether_type)) {
        iphdr *ip = reinterpret_cast<iphdr *> (first);
        first += sizeof(iphdr);

        if (ip->daddr != s_config._iface_ip)
            return RET::WRONG_ADDR;

        switch (ip->protocol) {
            case IPPROTO_ICMP:
                icmp *icmp_h = reinterpret_cast<struct icmp *> (first);
                int icmplen = packet_size - (first - packet);
                if (icmplen < icmp_header_size)// need in each reinterpet cast
                    break;                // not enough to look at ICMP header

                first += icmp_header_size;

                if (icmp_h->icmp_type == ICMP_TIMXCEED &&
                    icmp_h->icmp_code == ICMP_TIMXCEED_INTRANS) {
                    if (icmplen < icmp_header_size + sizeof(struct ip))
                        break;            // not enough data to look at inner IP

                    route.push_back(ip->saddr);//next hop

                } else if (icmp_h->icmp_type == ICMP_ECHOREPLY) {
                    if (icmplen < icmp_header_size + sizeof(struct ip))
                        break;            // not enough data to look at inner IP

                    route.push_back(ip->saddr);//end point
                    end = true;
                }

        }
    }
    return RET::SUCCESS;
}
