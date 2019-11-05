#include <sys/clock.h>
#include <sys/proc.h>
#include <include/bzero.h>
#include <sys/pcpu.h>
#include <sys/lock.h>

#define MAX_TIMERS      3
time_t timer_pool[MAX_TIMERS];
time_t ticks;

typedef void (*clock_hook_t)(void);
static clock_hook_t clock_hook;


void
clock_default_hook(void)
{

}

void
clock_init(void)
{
    ticks = 0;
    bzero(timer_pool,MAX_TIMERS * sizeof(time_t));
    clock_hook = clock_default_hook();
    machine_isr_set(ISR_CLOCK,clock_handle);
}

static void
clock_handle(int cpuid)
{
    struct thread *td = PCPU_GET(cpuid)->pcpu_curthread;
    
    thread_lock(&td->td_mtx);
    td->td_profclock--;         /* run on cpu   */
    td->td_virtualclock++;      /* all time     */

    proc_ticks();
    clock_hook();
    timer_handle();

    if(td->td_profclock < 1) {
        td->td_flags |= TDS_RESCHUDLE;
    }
    thread_unlcok(&td->td_mtx);
}


clock_hook_t
clock_gethook(void)
{
    return (clock_hook);
}

time_t
clock_getticks(void)
{
    
}