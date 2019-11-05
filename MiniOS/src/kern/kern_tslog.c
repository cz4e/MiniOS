
#include <sys/types.h>
#include <mips/include/atomic.h>
#include <sys/tslog.h>

#ifndef TSLOGSIZE   
#define TSLOGSIZE   262144
#endif

static volatile unsigned long nrecs = 0;

static struct timestamp {
    void *td;
    int type;
    const char *f;
    const char *s;
    uint64_t tsc;
} timestamps[TSLOGSIZE];

void 
tslog(void *td, int type, const char *f, const char *s)
{
    uint64_t tsc = 1;
    unsigned long pos;

    pos = atomic_fetchadd_long(&nrecs,1);

    if(pos <= TSLOGSIZE) {
        timestamps[pos].td = td;
        timestamps[pos].type = type;
        timestamps[pos].f = f;
        timestamps[pos].s = s;
        timestamps[pos].tsc = tsc;
    }
}