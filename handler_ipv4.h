#ifndef SENDER_HANDLER_IPV4_H
#define SENDER_HANDLER_IPV4_H

#include <vector>
#include <netinet/in.h>

enum class RET{
    SUCCESS,
    RECV_ERROR,
    TIMEOUT,
    WRONG_ADDR
};

//RET recv();
RET process_arp();

RET process_icmp(std::vector<in_addr_t>& route, bool& end);
#endif //SENDER_HANDLER_IPV4_H
