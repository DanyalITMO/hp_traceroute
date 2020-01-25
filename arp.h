#ifndef SENDER_ARP_H
#define SENDER_ARP_H

#include <netinet/in.h>

namespace arp {

void send_request(in_addr_t dst_ip);

}
#endif //SENDER_ARP_H
