#include <sys/sched.h>
#include <sys/pcpu.h>

extern struct thread *curthread;

void
sched_insert(struct thread *td,int pri,int cpuid,flag_t flags)
{
    struct pcpu *pcpu = PCPU_GET(cpuid);
    struct thread *var;
    struct proc *p = td->td_proc;
    struct sigevent *pevent = (struct sigevent *)uma_alloc(sizeof(struct sigevent));

    pri = pri % PRIORITY_STEP;
    if(flags & (TDS_NEW | TDS_RUNNING | TDS_READY)) {
        LIST_INSERT_BY_KEY(td,var,
                                (&pcpu->pcpu_runq[pri]),
                                        td_runq,td_priority);
        td->td_state &= ~(TDS_NEW | TDS_READY);
        td->td_state |= TDS_RUNNING;
    } else if(flags & (TDS_SLEEPING)) {
        LIST_INSERT_BY_KEY(td,var,
                                (&pcpu->pcpu_sleepq[pri]),
                                        td,sleepq,td_priority);
        td->td_state  |= TDS_SLEEPING;
    } 
    pevent->se_signal |= SIGCHLD;
    // INSERT penvent to taillist;

    return ;
}


struct thread *
sched_choose(int cpuid,int queue,int pri)
{
    struct pcpu * pcpu = PCPU_GET(cpuid);
    struct thread *td;

    pri = pri % PRIORITY_STEP;
    switch(queue) {
        case SCHED_RUNQ:
            LIST_REMOVE_HEAD((&pcpu->pcpu_runq[pri]),td);   // undefined
            break;
        case SCHED_SLEEPQ:
            LIST_REMOVE_HEAD((&pcpu->pcpu_sleepq[pri],td));
            break;
    }

    return (td);
}

