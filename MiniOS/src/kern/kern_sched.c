#include <sys/sched.h>
#include <sys/pcpu.h>
#include <sys/lock.h>

struct thread *curthread;
struct pcpu *curpcpu;
extern struct pcpu cpu_pcpu[MAX_CPU];

static int sched_index[MAX_CPU];

void 
sched_startup(void)
{
    for(int i = 0; i < MAX_CPU; i++) {
        sched_index[i] = HIHEST_PRIORITY;
    }
}

void
sched_insert(struct thread *td,int pri,int cpuid,flag_t flags)
{
    struct pcpu *pcpu = &cpu_pcpu[cpuid];
    struct thread *var;
    struct proc *p = td->td_proc;


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


    return ;
}



static struct thread *
_sched_choose(int cpuid,int queue,int pri)
{
    struct pcpu * pcpu = PCPU_GET(cpuid);
    struct thread *td;

    pri = pri % PRIORITY_STEP;

    switch(queue) {
        case SCHED_RUNQ:
            PCPU_LOCK(&pcpu->pcpu_lock);
            LIST_REMOVE_HEAD((&pcpu->pcpu_runq[pri]),td);
            PCPU_UNLOCK(&pcpu->pcpu_lock);
            break;
        case SCHED_SLEEPQ:
            PCPU_LOCK(&pcpu->pcpu_lock);
            LIST_REMOVE_HEAD((&pcpu->pcpu_sleepq[pri]),td);
            PCPU_UNLOCK(&pcpu->pcpu_lock);
            break;
    }

    return (td);
}


struct thread *
sched_choose(int cpuid, int queue)
{
    int pri = sched_index[cpuid];;
    struct thread *td;

    while(1) {
        if(pri > 0 ) pri = 63;
        td = _sched_choose(cpuid, queue, pri);
        if(td != NULL) {
            sched_index[cpuid] = pri;
            break;
        }
        pri--;
    }

    return (td);
}

void
sched_switch(void)
{
    static int pri = 63;
    struct thread *old_td, *new_td;
    struct pcb *curpcb;
    int cpuid;
    struct mtx td_lock;

    old_td = curthread;
    cpuid = CURPCPU_GET(cpuid);
    new_td = sched_choose(cpuid,SCHED_RUNQ);

    if(new_td) return;

    THREAD_LOCK(&new_td->td_lock);
    old_td->td_state &= ~(TDS_RUNNING );
    old_td->td_state |= (TDS_READY);
    THREAD_LOCK(&new_td->td_lock);

    THREAD_LOCK(&new_td->td_lock);
    new_td->td_state &= ~(TDS_READY | TDS_NEW | TDS_SLEEPING );
    new_td->td_state |= (TDS_RUNNING);
    THREAD_LOCK(&new_td->td_lock);
    cpu_switch(old_td, new_td,td_lock);
    


    curpcpu = &cpu_pcpu[new_td->td_cpuid];
    curthread = new_td;
    curpcb = new_td->td_pcb;

    pcpu_setcurpcb(cpuid, curpcb);
    pcpu_setcurthread(cpuid, curthread);
    pcpu_setcurproc(cpuid, new_td->td_proc);
}