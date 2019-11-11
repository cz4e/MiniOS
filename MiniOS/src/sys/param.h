#ifndef _SYS_PARAM_H_
#define _SYS_PARAM_H_

#include <sys/types.h>

static int 
is_round_base2(uint64_t n)
{
    uint64_t m = 1, p = 0;

    if(n <= 0 )
        return (0);
    else {
        while(m < n) {
            m *= 2;
            p++;
        }
        if(m == n) {
            return (n);
        } else {
            return (0);
        }
    }
}

static uint64_t 
powerof2(uint64_t n)
{
    uint64_t f = 1;
    while(n--) f *= 2;

    return (f);
}

static uint64_t 
round_base2(uint64_t n)
{
    uint64_t f = 1 ,counter = 1;
    while(f < n) f = powerof2(counter++);
    return (f == 1 ? 2 : f);
}

#endif