#ifndef __LIBDOSE_DEBUG_IMPL_H__
#define __LIBDOSE_DEBUG_IMPL_H__

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dose_vdebugf(const char* func, const char* format, va_list args) {
#ifdef DOSE_DEBUG
    fprintf(stderr, "[DOSE] ");
    fprintf(stderr, "%s: ", func);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
#endif
}

void dose_debugf(const char* func, const char* format, ...) {
#ifdef DOSE_DEBUG
    va_list args;
    va_start(args, format);
    dose_vdebugf(func, format, args);
    va_end(args);
#endif
}

#endif
