#ifndef SENDER_ROUTE_H
#define SENDER_ROUTE_H

#include <optional>
#include <vector>

struct route_info{
    std::optional<in_addr_t> _gw;
    int _iface_id{0};
    uint32_t _mask{0};
};

std::vector<route_info> get_routes(uint32_t addr);


#endif //SENDER_ROUTE_H
