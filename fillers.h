//
// Created by root on 07.01.2020.
//

#ifndef SENDER_FILLERS_H
#define SENDER_FILLERS_H

#include <unistd.h>
#include <array>
#include <net/if.h>
#include <netinet/in.h>

void fill_ethernet(char*& first, std::array<uint8_t, IFHWADDRLEN> const& dst, std::array<uint8_t, IFHWADDRLEN> const& src);
void fill_ip(char*& first, char* packet, in_addr_t dst, in_addr_t src, std::size_t packet_size);
void fill_icmp(char*& first, char* packet, std::size_t packet_size);

#endif //SENDER_FILLERS_H
