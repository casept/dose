#ifndef __LIBDOSE_ERROR_H__
#define __LIBDOSE_ERROR_H__

char *dose_getoserrstr(int os_errcode);
void dose_freeoserrstr(char *err);
int dose_getoserrcode(void);
char *dose_geterrstr(int dose_errcode);
void dose_freeerrstr(char *err);

enum dose_err_t {
    DOSE_ERR_OK = 0,
    DOSE_ERR_OS = -1,
    DOSE_ERR_ADDRINFO = -2,
    DOSE_ERR_PARSE = -3,
    DOSE_ERR_ALLOC = -4,
    DOSE_ERR_BUG = -5,
};

#endif
