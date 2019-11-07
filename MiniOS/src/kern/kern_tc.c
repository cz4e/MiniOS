#include <sys/tc.h>


static cpu_tick_f *cpu_ticks;
static uint64_t cpu_tick_freq;
static int cpu_tick_var;


uint64_t 
cpu_default_ticker(void)
{
    return (0);
}

void
set_cputicker(cpu_tick_f *handler, uint64_t freq, unsigned var)
{
    if(handler == NULL) {
        cpu_ticks = cpu_default_ticker;
    } else {
        cpu_ticks = handler;
        cpu_tick_freq = freq;
        cpu_tick_var = var;
    }
}

