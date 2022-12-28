#ifndef __LIBDOSE_ERROR_IMPL_H__
#define __LIBDOSE_ERROR_IMPL_H__

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    // So MSVC links against winsock2 automatically
    #pragma comment(lib, "ws2_32.lib")
#else
    // Assume Unix-like
    #include <errno.h>
    #include <error.h>
    #include <string.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

#define DOSE_ERRSTR_SIZE 512

const char* __DOSE_ERRSTR_OK = "no error";
const char* __DOSE_ERRSTR_OS = "operating system error: %s";
// TODO: Implement passing of error code to here and formatting the string according to it
const char* __DOSE_ERRSTR_ADDRINFO = "getaddrinfo error: TODO";
const char* __DOSE_ERRSTR_PARSE = "failed to parse addr";
const char* __DOSE_ERRSTR_ALLOC = "failed to allocate buffer";
const char* __DOSE_ERRSTR_BUG = "internal libodse bug, report to maintainers";
const char* __DOSE_ERRSTR_UNKNOWN = "unknown error code";

static char* __dose_strdup(const char* src) {
    char* dst = malloc(strlen(src) + 1);
    if (dst == NULL) return NULL;
    strcpy(dst, src);
    return dst;
}

char* dose_geterrstr(int dose_errcode) {
    enum dose_err_t err = (enum dose_err_t)dose_errcode;
    switch (err) {
        case DOSE_ERR_OK:
            return __dose_strdup(__DOSE_ERRSTR_OK);
        case DOSE_ERR_OS: {
            // Special, extract OS error message
            char* msg = malloc(DOSE_ERRSTR_SIZE);
            snprintf(msg, DOSE_ERRSTR_SIZE, __DOSE_ERRSTR_OS, dose_getoserrstr(dose_getoserrcode()));
            if (msg == NULL) {
                return NULL;
            }
            return msg;
        }
        case DOSE_ERR_ADDRINFO:
            return __dose_strdup(__DOSE_ERRSTR_ADDRINFO);
        case DOSE_ERR_PARSE:
            return __dose_strdup(__DOSE_ERRSTR_PARSE);
        case DOSE_ERR_ALLOC:
            return __dose_strdup(__DOSE_ERRSTR_ALLOC);
        case DOSE_ERR_BUG:
            return __dose_strdup(__DOSE_ERRSTR_BUG);
        default:
            return __dose_strdup(__DOSE_ERRSTR_UNKNOWN);
    }
}

void dose_freeoserrstr(char* errstr) { free(errstr); }

void dose_freeerrstr(char* errstr) { free(errstr); }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
int dose_getoserrcode(void) { return WSAGetLastError(); }
char* dose_getoserrstr(int errcode) {
    // TDOO: Check if dwLanguageId = 0 is reasonable (AKA English)
    char* errstr = malloc(DOSE_ERRSTR_SIZE);
    FormatMessgeA(FORMAT_MESSAGE_FROM_SYSTEM, errcode, errcode, 0, errstr, DOSE_ERRSTR_SIZE, NULL);
    return errstr;
}
#else
int dose_getoserrcode(void) { return errno; }
char* dose_getoserrstr(int errcode) {
    char* errstr = malloc(DOSE_ERRSTR_SIZE);
    strerror_r(errcode, errstr, DOSE_ERRSTR_SIZE);
    return errstr;
}
#endif

#endif
