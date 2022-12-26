#ifndef __LIBDOSE_TOPLEVEL_H__
#define __LIBDOSE_TOPLEVEL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

int dose_dial(const char *addr);
int dose_close(int desc);
ssize_t dose_send(int desc, const void *buf, size_t count);
ssize_t dose_recv(int desc, void *buf, size_t count);
// TODO: Consider exposing the addr parsing function as well

#ifdef DOSE_IMPLEMENTATION
    #include "dial_impl.h"
    #include "io_impl.h"
#endif

#ifdef __cplusplus
}
#endif

#endif
