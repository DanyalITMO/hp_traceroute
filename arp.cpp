
#include "arp.h"
#include "config.h"
#include <netinet/in.h>
#include "utils.h"
#include "fillers.h"
#include <sys/socket.h>

namespace arp {

void send_request(in_addr_t dst_ip){
    char *first = packet;
    fill_ethernet(first,  macaddr_t{0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF}, s_config._iface_mac, ETH_P_ARP);
    fill_arp_request(first, s_config._iface_mac, dst_ip, s_config._iface_ip);
    if (sendto(send_socket, packet, first - packet, 0,
               (struct sockaddr *) &send_socket_addr, (socklen_t) sizeof(sockaddr_ll)) < 0)
        perror("main:");
}

}
