#ifndef SENDER_STATES_H
#define SENDER_STATES_H

enum class STATES {
    INIT,
    RESOLVE_ARP,
    GET_PATH
};

struct resolve_arp_data{
    bool _resolved;
    uint32_t _time;
};

struct get_path_data{
    bool _reached_end;
    uint32_t time;
    std::vector<in_addr_t> _path;
};

#endif //SENDER_STATES_H
