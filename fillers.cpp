#include "fillers.h"

#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <csignal>
#include "stats.h"
#include <netinet/in.h>
#include "utils.h"

void fill_ethernet(char*& first, std::array<uint8_t, IFHWADDRLEN> const& dst, std::array<uint8_t, IFHWADDRLEN> const& src){
    ether_header *eh = (struct ether_header *) first;
    first += sizeof(ether_header);

    memcpy(eh->ether_dhost, &dst[0], IFHWADDRLEN);
    memcpy(eh->ether_shost, &src[0], IFHWADDRLEN);
    eh->ether_type = htons(ETH_P_IP);
}

void fill_ip(char*& first, char* packet,  in_addr_t dst, in_addr_t src, std::size_t packet_size){
    iphdr *ip = (struct iphdr *) (first);
    first += sizeof(iphdr);

    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 0;
    ip->tot_len = htons(packet_size - (first - packet) + sizeof(iphdr));
    ip->frag_off = 0;
    ip->ttl = 64;
    ip->protocol = IPPROTO_ICMP; // this has to be IPPROTO_RAW
    ip->check = 0;
    ip->saddr = src;
    ip->daddr = dst;
    ip->check = in_cksum((u_short *) ip, sizeof(iphdr));
}

void fill_icmp(char*& first, char* packet, std::size_t packet_size){
    auto len = packet_size - (first - packet);
    icmp *icmp = (struct icmp *) first;
    first += sizeof(icmp);

    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_id = 1;
    icmp->icmp_seq = 1;
    icmp->icmp_cksum = 0;
    icmp->icmp_cksum = in_cksum((u_short *) icmp, len);
}
