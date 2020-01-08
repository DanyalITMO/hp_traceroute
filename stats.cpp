#include <fstream>
#include <sstream>
#include "stats.h"

/*
std::ostream&    operator<< (std::ostream& stream, const stats_data& arg) {
    stream<<arg
}*/

stats_data operator-(stats_data const &arg1, stats_data const &arg2) {
    stats_data r;
    r.tx_packets = arg1.tx_packets - arg2.tx_packets;
    r.rx_packets = arg1.rx_packets - arg2.rx_packets;
    r.tx_bytes = arg1.tx_bytes - arg2.tx_bytes;
    r.rx_bytes = arg1.rx_bytes - arg2.rx_bytes;

    return r;
}

std::size_t get_param(std::string_view param) {
    std::ifstream myfile;
    std::stringstream buffer;

    myfile.open(std::string{"/sys/class/net/"}+ interface_name + std::string{"/statistics/"} + std::string{param});
    buffer << myfile.rdbuf();
    std::string t = buffer.str();
    auto now = std::stoull(t);
    myfile.close();
    return now;
}

stats_data get_all_params() {
    stats_data res;
    res.tx_packets = get_param("tx_packets");
    res.rx_packets = get_param("rx_packets");
    res.tx_bytes = get_param("tx_bytes");
    res.rx_bytes = get_param("rx_bytes");
    return res;
}

std::tuple<double, std::string> get_max_measurement(std::size_t number) {
    constexpr std::size_t gb = 1024 * 1024 * 1024;
    constexpr std::size_t mb = 1024 * 1024;
    constexpr std::size_t kb = 1024;
//    float ret_num;//{number};
//        ret_num = ;
    if (number > gb)
        return {static_cast<double>(number) / gb, "gbit/s"};
    else if (static_cast<double>(number) > mb)
        return {static_cast<double>(number) / mb, "mbit/s"};
    else if (number > kb)
        return {static_cast<double>(number) / kb, "kbit/s"};
    else
        return {number, "bit/s"};
}

/*
int main(int argc, char **argv) {

    stats_data previous;
    while (true) {
        sleep(1);
        auto now = get_all_params();
        std::cout<<"-------------NIC stats-------------"<<"\n";
        std::cout << "dTX packets: " << now.tx_packets - previous.tx_packets << "\n";
        std::cout << "dRX packets: " << now.rx_packets - previous.rx_packets << "\n";
//        std::cout << "dTX bytes: " << now.tx_bytes - previous.tx_bytes << "\n";
//        std::cout << "dRX bytes: " << now.rx_bytes - previous.rx_bytes << std::endl;
        {
            auto[n, s] = get_max_measurement(now.tx_bytes - previous.tx_bytes);
            std::cout << "dTX: " << n << s << "\n";
        }
        {
            auto[n, s] = get_max_measurement(now.rx_bytes - previous.rx_bytes);
            std::cout << "dRX: " << n << s << "\n";
        }
        previous = now;
    }

}*/