#ifndef _SYS_TIMEVAR_H_
#define _SYS_TIMEVAR_H_

#include <sys/types.h>

struct timevar {
    time_t          tv_second;
    utime_t         tv_usecond;
};

#endif