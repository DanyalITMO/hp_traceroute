#ifndef SENDER_CONFIG_H
#define SENDER_CONFIG_H

#include "utils.h"

enum class STATE{
    RESOLVE_ARP,
    GET_PATH,
    GENERATE_LOAD,
    FINISH
};
struct config {
    in_addr_t _dst_ip;
    in_addr_t _iface_ip;
    std::string _iface_name;
    int _iface_index;
    macaddr_t _iface_mac;
    macaddr_t _next_hop_mac = {0};
    in_addr_t _next_hop_ip = {0};
    std::size_t _payload_size;
    STATE _state{STATE::RESOLVE_ARP};
};

extern config s_config;

#endif //SENDER_CONFIG_H
