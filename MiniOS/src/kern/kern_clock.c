#include <sys/cdefs.h>
#include <sys/clock.h>
#include <sys/proc.h>
#include <include/bzero.h>
#include <sys/pcpu.h>
#include <sys/system.h>
#include <sys/tslog.h>

uint64_t counter_freq;
static uint64_t cycles_per_usec;
static struct time clock_timers[MAX_TIMES];

typedef void( *tick_hook_t)(void);
static tick_hook_t clock_ticks_hook;

void
mips_timer_early_init(uint64_t clock_hz)
{
    counter_freq = clock_hz;
    cycles_per_usec = (clock_hz / (1000 * 1000));
    bzero(clock_timers, sizeof(struct time) * MAX_TIMERS);
    clock_ticks_hook = clock_default_ticks_hook();
    
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

int
mips_do_clock(void)
{
    struct thread *td;
    td = curthread;

    ++td->td_runtime;
    --td->td_cputime;

    sched_addticks_propogation(td->cpuid,1);

    if(td->td_cputime <= 0) {
        td->td_runtime = 0;
        td->td_priority = (td->td_priority / (td->td_runtime + 1)) * td->td_waittime 
                                    + td->td_waittime;
        sched_switch();
    }

    return ;
}