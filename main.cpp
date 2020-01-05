#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <linux/if_ppp.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <string_view>
#include <netinet/ip_icmp.h>
#include <linux/if_ether.h>
#include <netinet/ether.h>
#include <csignal>
#include "stats.h"
#include <netinet/in.h>

extern std::string interface_name {"veth0"};

in_addr_t get_ip(std::string_view name)
{
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

void sig_alrm(int signo) {
    static stats_data previous;
    auto now = get_all_params();
    std::cout << "-------------NIC stats-------------" << "\n";
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

    alarm(1);
    return;
}
/*
void fill_ethernet(char* first){

}*/
int main(int argc, char **argv) {
    signal(SIGALRM, sig_alrm);
    sig_alrm(SIGALRM);		/* send first packet */

    int s;
    struct sockaddr_in dst_addr;
    char packet[50];
    char* first = packet;

    if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
//    if((s = socket(AF_PACKET, SOCK_RAW, 0/*htons(ETH_P_ALL)*/)) < 0) {
        perror("error:");
        exit(EXIT_FAILURE);
    }

    dst_addr.sin_family = AF_INET;
    dst_addr.sin_port = 0; /* not needed in SOCK_RAW */
    dst_addr.sin_addr.s_addr = get_ip("veth1");

//    inet_pton(AF_INET, "127.0.0.1", (struct in_addr *) &dst_addr.sin_addr.s_addr);
    memset(dst_addr.sin_zero, 0, sizeof(dst_addr.sin_zero));

    memset(packet, 'A', sizeof(packet));   /* payload will be all As */
/*
    //ethernet
    ether_header *eh = (struct ether_header *) first;
    first += sizeof(ether_header);
    memset(eh->ether_dhost, ETH_ALEN, 0xFF);
    memset(eh->ether_shost, ETH_ALEN, 0xFF);
    eh->ether_type = htons(ETH_P_IP);
*/
    //ip
    iphdr *ip = (struct iphdr *) (first);
    first += sizeof(iphdr);

    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 0;
    ip->tot_len = htons(sizeof(packet) - (first - packet) + sizeof(iphdr));
    ip->frag_off = 0;  /* NF */
    ip->ttl = 64;
    ip->protocol = IPPROTO_ICMP; /* this has to be IPPROTO_RAW */
    ip->check = 0;
    ip->saddr = dst_addr.sin_addr.s_addr;
    ip->daddr = dst_addr.sin_addr.s_addr;

//icmp
    icmp *icmp = (struct icmp *) first;
    first += sizeof(icmp);

    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_id = 1;
    icmp->icmp_seq = 1;
//    memset(icmp->icmp_data, 0xa5, datalen);	/* fill with pattern */
//    Gettimeofday((struct timeval *) icmp->icmp_data, NULL);

//    len = 8 + datalen;		/* checksum ICMP header and data */
    icmp->icmp_cksum = 0;
//    icmp->icmp_cksum = in_cksum((u_short *) icmp, len);;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    addr.sin_addr.s_addr = get_ip("veth0");
//    inet_pton(AF_INET, "192.168.11.10", (struct in_addr *) &addr.sin_addr.s_addr);

    if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(2);
    }

    while (42) {
//        sleep(1);
        if (sendto(s, packet, sizeof(packet), 0,
                   (struct sockaddr *) &dst_addr, (socklen_t) sizeof(dst_addr)) < 0)
            perror("uh oh:");
    }
    return (0);
}



