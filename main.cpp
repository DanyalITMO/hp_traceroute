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
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string_view>
#include <tuple>
#include <netinet/ip_icmp.h>
#include <linux/if_ether.h>
struct stats_data{
    std::size_t tx_packets;
    std::size_t rx_packets;
    std::size_t tx_bytes;
    std::size_t rx_bytes;
};
/*
std::ostream& operator<< (std::ostream& stream, const stats_data& arg) {
    stream<<arg
}*/

stats_data operator-(stats_data const& arg1, stats_data const& arg2){
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

    myfile.open(std::string{"/sys/class/net/wlo1/statistics/"} + std::string{param});
    buffer << myfile.rdbuf();
    std::string t = buffer.str();
    auto now = std::stoull(t);
    myfile.close();
    return now;
}

stats_data get_all_params(){
    stats_data res;
    res.tx_packets = get_param("tx_packets");
    res.rx_packets = get_param("rx_packets");
    res.tx_bytes = get_param("tx_bytes");
    res.rx_bytes = get_param("rx_bytes");
    return res;
}

std::tuple<double, std::string> get_max_measurement(std::size_t number){
    constexpr std::size_t gb = 1024 * 1024 * 1024;
    constexpr std::size_t mb = 1024 * 1024;
    constexpr std::size_t kb = 1024;
//    float ret_num;//{number};
//        ret_num = ;
    if(number > gb)
        return {static_cast<double>(number) / gb, "gb"};
    else if(static_cast<double>(number) > mb)
        return {static_cast<double>(number) / mb, "mb"};
    else if(number > kb)
        return {static_cast<double>(number) / kb, "kb"};
    else
        return {number, "b"};
}/*
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
/*{

    ioctl(5, SIOCGIFFLAGS, {ifr_name="eth0",    ifr_flags=IFF_UP|IFF_BROADCAST|IFF_RUNNING|IFF_MULTICAST}) = 0
    ioctl(5, SIOCGIFHWADDR, {ifr_name="eth0", ifr_hwaddr=84:2b:2b:b7:9e:6d}) = 0
    ioctl(5, SIOCGIFMETRIC, {ifr_name="eth0", ifr_metric=0}) = 0
    ioctl(5, SIOCGIFMTU, {ifr_name="eth0", ifr_mtu=1500}) = 0

    FILE *fp;

    char returnData[64];

    fp = popen("/sbin/ifconfig wlo1", "r");

    while (fgets(returnData, 64, fp) != NULL)
    {
        printf("%s", returnData);
    }

    pclose(fp);
}*/


//int main(int argc, char **argv){
//
//
//
//    int s;
//    struct sockaddr_in dst_addr;
//    char packet[50];
//
//    struct iphdr *ip = (struct iphdr *)packet;
//
////    if((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
//    if((s = socket(AF_PACKET, SOCK_RAW, 0/*htons(ETH_P_ALL)*/)) < 0) {
//        perror("error:");
//        exit(EXIT_FAILURE);
//    }
//
//    dst_addr.sin_family = AF_INET;
//    dst_addr.sin_port = 0; /* not needed in SOCK_RAW */
//    inet_pton(AF_INET, "127.0.0.1", (struct in_addr *)&dst_addr.sin_addr.s_addr);
//    memset(dst_addr.sin_zero, 0, sizeof(dst_addr.sin_zero));
//
//    memset(packet, 'A', sizeof(packet));   /* payload will be all As */
//
//    ip->ihl = 5;
//    ip->version = 4;
//    ip->tos = 0;
//    ip->tot_len = htons(40);
//    ip->frag_off = 0;  /* NF */
//    ip->ttl = 64;
//    ip->protocol = IPPROTO_ICMP; /* this has to be IPPROTO_RAW */
//    ip->check = 0;
//    ip->saddr = dst_addr.sin_addr.s_addr;
//    ip->daddr = dst_addr.sin_addr.s_addr;
//
//
//    struct icmp	*icmp;
//    icmp = (struct icmp *) (packet + sizeof(struct iphdr));
//    icmp->icmp_type = ICMP_ECHO;
//    icmp->icmp_code = 0;
//    icmp->icmp_id = 1;
//    icmp->icmp_seq = 1;
////    memset(icmp->icmp_data, 0xa5, datalen);	/* fill with pattern */
////    Gettimeofday((struct timeval *) icmp->icmp_data, NULL);
//
////    len = 8 + datalen;		/* checksum ICMP header and data */
//    icmp->icmp_cksum = 0;
////    icmp->icmp_cksum = in_cksum((u_short *) icmp, len);
//
//
//    while(42) {
////        sleep(5);
//        if (sendto(s, packet, sizeof(packet), 0,
//                   (struct sockaddr *)&dst_addr, (socklen_t)sizeof(dst_addr)) < 0)
//            perror("uh oh:");
//    }
//    return(0);
//}

