#include <sys/cdefs.h>
#include <sys/clock.h>
#include <sys/proc.h>
#include <include/bzero.h>
#include <sys/pcpu.h>
#include <sys/system.h>
#include <sys/tslog.h>

uint64_t counter_freq;
static uint64_t cycles_per_usec;

void
mips_timer_early_init(uint64_t clock_hz)
{
    counter_freq = clock_hz;
    cycles_per_usec = (clock_hz / (1000 * 1000));
}

void
mips_delay(int ms)
{
    uint32_t cur, last, delta, usecs;

    TSENTER();
    last = mips_rd_count();
    delta = usecs = 0;

    while(ms > usecs) {
        cur = mips_rd_count();
        if(cur < last)
            delta += cur + (0xffffffff - last) + 1;
        else
            delta += cur - last;
        last = cur;

        if(delta >= cycles_per_usec) {
            usecs += delta / cycles_per_usec;
            delta %= cycles_per_usec;
        }
    }
    TSEXIT();
}

void 
mips_timer_init_params(uint64_t platfrom_counter_freq, int double_count)
{
    counter_freq = platfrom_counter_freq;
    if(double_count != 0)
        counter_freq /= 2;
    cycles_per_usec = counter_freq / (1000 * 1000);
    set_cputicker(tick_ticker, counter_freq, 1);
}