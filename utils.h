
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
#include <linux/if_packet.h>
#include <vector>
#include <map>

constexpr std::size_t icmp_header_size{8};

using macaddr_t = std::array<uint8_t, IFHWADDRLEN>;

macaddr_t get_mac_from_iface(std::string_view ifname);

in_addr_t get_ip_from_iface(std::string_view name);

u_short in_cksum(u_short *addr, int len);

extern thread_local char packet[65536];

extern sockaddr_ll send_socket_addr;

extern sockaddr_ll recv_socket_addr;

extern int send_socket;

extern int recv_socket;

//extern std::map<in_addr_t, std::size_t> statistic;

#endif //SENDER_UTILS_H
