#ifndef SENDER_CONFIG_H
#define SENDER_CONFIG_H

#include "utils.h"

struct config {
    in_addr_t _dst_ip;
    in_addr_t _iface_ip;
    std::string _iface_name;
    int _iface_index;
    macaddr_t _iface_mac;
    macaddr_t _next_hop_mac;
    std::size_t _payload_size;
};

extern config s_config;

#endif //SENDER_CONFIG_H
