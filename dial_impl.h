#ifndef __LIBDOSE_DIAL_IMPL_H__
#define __LIBDOSE_DIAL_IMPL_H__

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    // Order matters
    // clang-format off
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <ws2tcpip.h>
    // clang-format on
    // So MSVC links against winsock2 automatically
    #pragma comment(lib, "ws2_32.lib")
#else
    // Assume Unix-like
    #include <arpa/inet.h>
    #include <errno.h>
    #include <netdb.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <unistd.h>
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "debug_impl.h"
#include "error.h"

enum dose_net_t {
    DOSE_NET_TCP = 0,
    DOSE_NET_UDP = 1,
};

static int __dose_parse_addr(const char* addr, char** scratch, enum dose_net_t* net, char** netaddr, char** service) {
    // TODO: Propagate errors

    char* seg_0 = NULL;
    char* seg_1 = NULL;
    char* seg_2 = NULL;

    // First segment is special (always assumed to exist)
    if (strncmp(addr, "", 1) == 0) {
        dose_debugf(__func__, "FAIL: Address cannot be empty");
        return DOSE_ERR_PARSE;
    }

    // Separate by "!"
    *scratch = calloc(strlen(addr) + 1, sizeof(char));
    if (*scratch == NULL) {
        dose_debugf(__func__, "FAIL: Failed to allocate scratch buffer");
        return DOSE_ERR_ALLOC;
    }
    strncpy(*scratch, addr, strlen(addr));
    seg_0 = strtok(*scratch, "!");
    seg_1 = strtok(NULL, "!");
    if (seg_1 != NULL) {
        seg_2 = strtok(NULL, "!");
    }
    if (seg_0 == NULL) {
        dose_debugf(__func__, "FAIL: network part of address is empty");
        return DOSE_ERR_PARSE;
    }
    if (seg_1 == NULL) {
        dose_debugf(__func__, "FAIL: addr part of address is empty");
        return DOSE_ERR_PARSE;
    }
    if (seg_2 == NULL) {
        dose_debugf(__func__, "FAIL: service part of address is empty");
        return DOSE_ERR_PARSE;
    }

    if (strcmp(seg_0, "tcp") == 0) {
        *net = DOSE_NET_TCP;
    } else if (strcmp(seg_0, "udp") == 0) {
        *net = DOSE_NET_UDP;
    } else {
        dose_debugf(__func__, "FAIL: Illegal address due to unknown network part: %s", seg_0);
        return DOSE_ERR_PARSE;
    }
    *netaddr = seg_1;
    *service = seg_2;

    return 0;
}

int dose_dial(const char* addr) {
    // Parse address
    char *scratch, *service, *netaddr;
    enum dose_net_t net;
    int parse_err = __dose_parse_addr(addr, &scratch, &net, &netaddr, &service);
    if (parse_err != 0) {
        dose_debugf(__func__, "FAIL: Address parsing failed");
        free(scratch);
        return parse_err;
    }

    // Perform socket library initialization

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    LPWSADATA data;
    memset(&data, 0, sizeof(LPWSADATA));
    if (WSAStartup(MAKEWORD(2, 2), data) != 0) {
        dose_debugf(__func__, "FAIL: WSAStartup() failed");
        free(scratch);
        return DOSE_ERR_OS;
    }
#endif

    // Turn netaddr into actually useful address
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    switch (net) {
        case DOSE_NET_TCP:
            hints.ai_socktype = SOCK_STREAM;
            break;
        case DOSE_NET_UDP:
            hints.ai_socktype = SOCK_DGRAM;
            break;
        default:
            dose_debugf(__func__, "BUG: net had unknown value %d", net);
            free(scratch);
            return DOSE_ERR_BUG;
    }
    // TODO: Figure out how to pass this error to the user
    int ai_err = getaddrinfo(netaddr, service, &hints, &res);
    if (ai_err != 0) {
        dose_debugf(__func__, "FAIL: getaddrinfo() failed: %s", gai_strerror(ai_err));
        free(scratch);
        return DOSE_ERR_ADDRINFO;
    }

    int sock;
    // Try to connect to any address we obtained
    for (p = res; p != NULL; p = p->ai_next) {
        if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            // TODO: Print failed address using inet_ntop
            dose_debugf(__func__, "INFO: Address failed, trying next");
            continue;
        }

        if (connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
            dose_debugf(__func__, "FAIL: connect() failed: %s", strerror(errno));
            close(sock);
            continue;
        }

        break;  // if we get here, we must have connected successfully
    }

    if (p == NULL) {
        // looped off the end of the list with no connection
        dose_debugf(__func__, "FAIL: Exhausted all addresses without connection");
        freeaddrinfo(res);
        free(scratch);
        return DOSE_ERR_OS;
    }

    freeaddrinfo(res);
    free(scratch);
    return sock;
}
#endif
