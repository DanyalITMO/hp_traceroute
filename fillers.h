//
// Created by root on 07.01.2020.
//

#ifndef SENDER_FILLERS_H
#define SENDER_FILLERS_H

#include <unistd.h>
#include <array>
#include <net/if.h>
#include <netinet/in.h>
#include "utils.h"
#include <netinet/in.h>
#include <linux/types.h>

struct my_arphdr {
    __be16		ar_hrd;		/* format of hardware address	*/
    __be16		ar_pro;		/* format of protocol address	*/
    unsigned char	ar_hln;		/* length of hardware address	*/
    unsigned char	ar_pln;		/* length of protocol address	*/
    __be16		ar_op;		/* ARP opcode (command)		*/

    unsigned char		ar_sha[ETH_ALEN];	/* sender hardware address	*/
    unsigned char		ar_sip[4];		/* sender IP address		*/
    unsigned char		ar_tha[ETH_ALEN];	/* target hardware address	*/
    unsigned char		ar_tip[4];		/* target IP address		*/

};

void fill_ethernet(char*& first, std::array<uint8_t, IFHWADDRLEN> const& dst, std::array<uint8_t, IFHWADDRLEN> const& src, uint16_t proto);
void fill_arp_request(char*& first, macaddr_t const& src_mac, in_addr_t dst_ip, in_addr_t src_ip);
void fill_ip(char*& first, in_addr_t dst, in_addr_t src, std::size_t packet_size, uint8_t ttl = 64);
void fill_icmp(char*& first, std::size_t packet_size);

#endif //SENDER_FILLERS_H
