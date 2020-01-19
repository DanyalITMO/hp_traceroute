
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

macaddr_t get_mac_from_iface(int s, std::string_view ifname){
    struct ifreq if_mac;
    memset(&if_mac, 0, sizeof(struct ifreq));
    strncpy(if_mac.ifr_name, ifname.data(), IFNAMSIZ-1);
    if (ioctl(s, SIOCGIFHWADDR, &if_mac) < 0)
        perror("SIOCGIFHWADDR");
    std::array<uint8_t, IFHWADDRLEN> src_mac;
    memcpy(&src_mac[0], (uint8_t *)&if_mac.ifr_hwaddr.sa_data, IFHWADDRLEN);
    return src_mac;
}

in_addr_t get_ip_from_iface(std::string_view name)
{

    //TODO if_indextoname // if_indextoname
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;
    /* I want IP address attached to "eth0" */
    strncpy(ifr.ifr_name, name.data(), IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    /* display result */
    printf("%s = %s\n",name.data(), inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    return ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr;
}

u_short in_cksum(u_short *addr, int len) {
    int nleft = len;
    u_short *w = addr;
    int sum = 0;
    u_short answer = 0;
/*
 * Our algorithm is simple, using a 32 bit accumulator (sum), we add
 * sequential 16 bit words to it, and at the end, fold back all the
 * carry bits from the top 16 bits into the lower 16 bits.
 */
    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }

/* mop up an odd byte, if necessary */
    if (nleft == 1) {
        *(u_char *) (&answer) = *(u_char *) w;
        sum += answer;
    }

/* add back carry outs from top 16 bits to low 16 bits */
    sum = (sum >> 16) + (sum & 0xffff);    /* add hi 16 to low 16 */
    sum += (sum >> 16);            /* add carry */
    answer = ~sum;                /* truncate to 16 bits */
    return (answer);
}
