#ifndef SENDER_STATS_H
#define SENDER_STATS_H

#include <string>
#include <tuple>
extern std::string interface_name;
struct stats_data {
    std::size_t tx_packets;
    std::size_t rx_packets;
    std::size_t tx_bytes;
    std::size_t rx_bytes;
};
stats_data operator-(stats_data const &arg1, stats_data const &arg2);

stats_data get_all_params();

std::tuple<double, std::string> get_max_measurement(std::size_t number);


#endif //SENDER_STATS_H
