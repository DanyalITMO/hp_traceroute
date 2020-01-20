
#ifndef SENDER_UTILS_H
#define SENDER_UTILS_H

#include <netinet/in.h>
#include <string_view>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <linux/if_ppp.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <string_view>
#include <netinet/ip_icmp.h>
#include <linux/if_ether.h>
#include <netinet/ether.h>
#include <csignal>
#include "stats.h"
#include <netinet/in.h>

constexpr  std::size_t icmp_header_size{8};
using macaddr_t = std::array<uint8_t, IFHWADDRLEN>;

macaddr_t get_mac_from_iface(std::string_view ifname);

in_addr_t get_ip_from_iface(std::string_view name);

u_short in_cksum(u_short *addr, int len);


#endif //SENDER_UTILS_H
