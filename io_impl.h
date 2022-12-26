#ifndef __LIBDOSE_IO_IMPL_H__
#define __LIBDOSE_IO_IMPL_H__

#include <stdint.h>
#include <sys/socket.h>

ssize_t dose_send(int desc, const void *buf, size_t count) {
    // TODO: Deal w/Win32 and their magic socket read/write functions
    return send(desc, buf, count, 0);
}

ssize_t dose_recv(int desc, void *buf, size_t count) {
    // TODO: Deal w/Win32 and their magic socket read/write functions
    return recv(desc, buf, count, 0);
}
#endif
