#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/rtnetlink.h>
#include <iostream>
#include <limits>
#include <bitset>
#include <optional>
#include <vector>
#include "route.h"

int rtnl_receive(int fd, struct msghdr *msg, int flags)
{
    int len;

    do {
        len = recvmsg(fd, msg, flags);
    } while (len < 0 && (errno == EINTR || errno == EAGAIN));

    if (len < 0) {
        perror("Netlink receive failed");
        return -errno;
    }

    if (len == 0) {
        perror("EOF on netlink");
        return -ENODATA;
    }

    return len;
}

static int rtnl_recvmsg(int fd, struct msghdr *msg, char **answer)
{
    struct iovec *iov = msg->msg_iov;
    char *buf;
    int len;

    iov->iov_base = NULL;
    iov->iov_len = 0;

    len = rtnl_receive(fd, msg, MSG_PEEK | MSG_TRUNC);

    if (len < 0) {
        return len;
    }

    buf = (char*)malloc(len);

    if (!buf) {
        perror("malloc failed");
        return -ENOMEM;
    }

    iov->iov_base = buf;
    iov->iov_len = len;

    len = rtnl_receive(fd, msg, 0);

    if (len < 0) {
        free(buf);
        return len;
    }

    *answer = buf;

    return len;
}

void parse_rtattr(struct rtattr *tb[], int max, struct rtattr *rta, int len)
{
    memset(tb, 0, sizeof(struct rtattr *) * (max + 1));

    while (RTA_OK(rta, len)) {
        if (rta->rta_type <= max) {
            tb[rta->rta_type] = rta;
        }

        rta = RTA_NEXT(rta,len);
    }
}

static inline int rtm_get_table(struct rtmsg *r, struct rtattr **tb)
{
    __u32 table = r->rtm_table;

    if (tb[RTA_TABLE]) {
        table = *(__u32 *)RTA_DATA(tb[RTA_TABLE]);
    }

    return table;
}

void print_bits(uint32_t t){
    std::bitset<32> x(t);
    std::cout << x << std::endl;
}

bool is_match(uint32_t dst, uint32_t mask, uint32_t ip_from_packet){
    return (ip_from_packet & mask) == dst;
}

std::optional<route_info> check_route(struct nlmsghdr* nl_header_answer, uint32_t ip_from_packet){
    struct rtmsg* r = (struct rtmsg*)NLMSG_DATA(nl_header_answer);
    int len = nl_header_answer->nlmsg_len;
    struct rtattr* tb[RTA_MAX+1];
    int table;
    char buf[256];

    len -= NLMSG_LENGTH(sizeof(*r));

    if (len < 0) {
        perror("Wrong message length");
        return {};
    }

    parse_rtattr(tb, RTA_MAX, RTM_RTA(r), len);

    table = rtm_get_table(r, tb);

    if (r->rtm_family != AF_INET && table != RT_TABLE_MAIN) {
        return {};
    }

    if (tb[RTA_DST] /*&& tb[RTA_GATEWAY]*/) {
        uint32_t mask = std::numeric_limits<uint32_t>::max();
//        print_bits(mask);
        mask =  mask << (32 - r->rtm_dst_len);
        mask = htonl(mask);

        std::optional<route_info> res;
        route_info ri;
        if(is_match(*((uint32_t *)RTA_DATA(tb[RTA_DST])), mask, ip_from_packet)){
            if(tb[RTA_GATEWAY])
                ri._gw = *((uint32_t *)tb[RTA_GATEWAY]);
            if (tb[RTA_OIF]) {
                ri._iface_id = *(__u32 *)RTA_DATA(tb[RTA_OIF]);
            }
            ri._mask = r->rtm_dst_len;
            res = ri;
            return  res;
//            std::cerr<<"!!match: "<<  inet_ntop(r->rtm_family, RTA_DATA(tb[RTA_DST]), buf, sizeof(buf))<<std::endl;
        }
    }
    return {};
//    printf("\n");
}

int open_netlink()
{
    struct sockaddr_nl saddr;

    int sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);

    if (sock < 0) {
        perror("Failed to open netlink socket");
        return -1;
    }

    memset(&saddr, 0, sizeof(saddr));

    saddr.nl_family = AF_NETLINK;
    saddr.nl_pid = getpid();

    if (bind(sock, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        perror("Failed to bind to netlink socket");
        close(sock);
        return -1;
    }

    return sock;
}

int do_route_dump_requst(int sock)
{
    struct {
        struct nlmsghdr nlh;
        struct rtmsg rtm;
    } nl_request;

    nl_request.nlh.nlmsg_type = RTM_GETROUTE;
    nl_request.nlh.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    nl_request.nlh.nlmsg_len = sizeof(nl_request);
    nl_request.nlh.nlmsg_seq = time(NULL);
    nl_request.rtm.rtm_family = AF_INET;

    return send(sock, &nl_request, sizeof(nl_request), 0);
}

std::vector<route_info> get_routes_from_msg(int sock, uint32_t addr)
{
    struct sockaddr_nl nladdr;
    struct iovec iov;
    struct msghdr msg = {
            .msg_name = &nladdr,
            .msg_namelen = sizeof(nladdr),
            .msg_iov = &iov,
            .msg_iovlen = 1,
    };

    char *buf;
    int dump_intr = 0;

    int status = rtnl_recvmsg(sock, &msg, &buf);

    struct nlmsghdr *h = (struct nlmsghdr *)buf;
    int msglen = status;

//    printf("Main routing table IPv4\n");

    std::vector<route_info> suitable_routes;
    while (NLMSG_OK(h, msglen)) {

//        std::cerr<<"delta: "<<(int*)h - (int*)buf<<std::endl;
        if (h->nlmsg_flags & NLM_F_DUMP_INTR) {
            fprintf(stderr, "Dump was interrupted\n");
            free(buf);
            return {};
        }

        if (nladdr.nl_pid != 0) {
            continue;
        }

        if (h->nlmsg_type == NLMSG_ERROR) {
            perror("netlink reported error");
            free(buf);
        }

        auto route = check_route(h, addr);
        if(route)
            suitable_routes.push_back(*route);
        h = NLMSG_NEXT(h, msglen);
    }

    free(buf);

    return suitable_routes;
}

std::vector<route_info> get_routes(uint32_t addr){
    int nl_sock = open_netlink();

    if (do_route_dump_requst(nl_sock) < 0) {
        perror("Failed to perfom request");
        close(nl_sock);
        return {};
    }

    auto res = get_routes_from_msg(nl_sock, addr);

    close (nl_sock);

    return res;
}
