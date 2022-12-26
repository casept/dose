#ifndef __LIBDOSE_IO_IMPL_H__
#define __LIBDOSE_IO_IMPL_H__

#include <unistd.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    // So MSVC links against winsock2 automatically
    #pragma comment(lib, "ws2_32.lib")
#else
    // Assume Unix-like
    #include <errno.h>
    #include <error.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <unistd.h>
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "debug_impl.h"

// Windows uses different types for the buffer
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
ssize_t dose_send(int desc, const void *buf, size_t count) { return send(desc, (const char *)buf, count, 0); }
ssize_t dose_recv(int desc, void *buf, size_t count) { return recv(desc, (char *)buf, count, 0); }
#else
ssize_t dose_send(int desc, const void *buf, size_t count) { return send(desc, buf, count, 0); }
ssize_t dose_recv(int desc, void *buf, size_t count) { return recv(desc, buf, count, 0); }
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
int dose_close(int desc) {
    if (closesocket(desc) != 0) {
        // TODO: Extract error string here as well
        dose_debugf(__func__, "FAIL: closesocket() failed");
        return -1;
    }
    while (true) {
        // We may have to try multiple times
        switch (WSACleanup()) {
            case 0:
                break;
            case WSAEINPROGRESS:
                continue;
            default:
                dose_debugf(__func__, "FAIL: WSACleanup() failed");
                return -1;
        }
    }
    return 0;
}
#else
int dose_close(int desc) {
    if (close(desc) != 0) {
        dose_debugf(__func__, "FAIL: close() failed: %s", strerror(errno));
        return -1;
    }

    return 0;
}
#endif

#endif
