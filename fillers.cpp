#include "fillers.h"

#include <sys/types.h>
#include "stats.h"
#include <cstring>
#include "utils.h"
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <netinet/udp.h>

void fill_ethernet(char*& first, std::array<uint8_t, IFHWADDRLEN> const& dst, std::array<uint8_t, IFHWADDRLEN> const& src, uint16_t proto){
    ether_header *eh = (struct ether_header *) first;
    first += sizeof(ether_header);

    memcpy(eh->ether_dhost, &dst[0], IFHWADDRLEN);
    memcpy(eh->ether_shost, &src[0], IFHWADDRLEN);
    eh->ether_type = htons(proto);
}

void fill_arp_request(char*& first, macaddr_t const& src_mac, in_addr_t dst_ip, in_addr_t src_ip){
    my_arphdr* arp = reinterpret_cast<my_arphdr*>(first);
    first += sizeof(my_arphdr);

    arp->ar_hrd =  htons(ARPHRD_ETHER);
    arp->ar_pro =  htons(ETH_P_IP);
    arp->ar_hln = IFHWADDRLEN;
    arp->ar_pln = sizeof(in_addr_t);
    arp->ar_op =  htons(ARPOP_REQUEST);

    memcpy(arp->ar_sha, &src_mac[0], IFHWADDRLEN);
    memset(arp->ar_tha, 0xFF, IFHWADDRLEN);

    auto* src = reinterpret_cast<in_addr_t*>(arp->ar_sip);
    *src =  src_ip;
    auto* dst = reinterpret_cast<in_addr_t*>(arp->ar_tip);
    *dst =  dst_ip;
}

void fill_ip(char*& first, in_addr_t dst, in_addr_t src, std::size_t packet_size, uint8_t ttl){
    iphdr *ip = (struct iphdr *) (first);
    first += sizeof(iphdr);

    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 0;
    ip->tot_len = htons(packet_size + sizeof(iphdr));
    ip->frag_off = 0;
    ip->ttl = ttl;
    ip->protocol = IPPROTO_ICMP; // this has to be IPPROTO_RAW
    ip->saddr = src;
    ip->daddr = dst;
    ip->check = 0;
    ip->check = in_cksum((u_short *) ip, sizeof(iphdr));
}

void fill_icmp(char*& first, std::size_t packet_size){
    icmp *icmp = (struct icmp *) first;
    first += icmp_header_size;

    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_id = 1;
    icmp->icmp_seq = 1;
    icmp->icmp_cksum = 0;
    icmp->icmp_cksum = in_cksum((u_short *) icmp, icmp_header_size + packet_size);
}

void fill_udp(char*& first, uint16_t dport, uint16_t sport){
    udphdr *udp = (struct udphdr *) first;
    first += sizeof(struct udphdr);

    udp->source = sport;
    udp->dest = dport;
    udp->check = 0;
    udp->check = in_cksum((u_short *) udp, sizeof(struct udphdr));
}

