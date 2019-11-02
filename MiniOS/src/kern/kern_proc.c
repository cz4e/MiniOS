#include <sys/proc.h>
#include <include/bzero.h>
#include <sys/uma.h>
#include <sys/pcpu.h>

#define DEFAULT_PRIORITY        100
#define SET_DEFAULT             -1
static pid_t bit_map = 0;

struct proc *curproc;
struct thread *curthread;
static struct proc *proc_idle;

static pid_t
get_vaild_pid(void)
{
    return (bit_map++);
}


void
proc_insert(struct proc *p,int cpuid)
{
    struct pcpu *pcpu = PCPU_GET(cpuid);
    LIST_INSERT_HEAD(pcpu,p,pcpu_list);
}

void 
proc_init(struct proc *p, struct thread *td, struct proc *parent,
             pid_t pid, flag_t flags, int pri,int cpuid,char *name)
{
    long i;
    bzero(p,sizeof(struct proc));
    p->p_threads.lh_first = td;
    p->p_parent = parent;
    p->p_pid = get_vaild_pid();
    p->p_ppid = pid;
    p->p_flags = flags;
    p->p_priority = pri == SET_DEFAULT ? DEFAULT_PRIORITY : pri;
    p->p_states |= PS_NEW;
    p->p_mtx = MTX_INITIALIZER;
    LIST_HEAD_INIT(p,p_childrens);
    proc_insert(p,cpuid);

    for(i = 0;i < MAX_NAME && name[i];i++) {
        p->p_name[i] = name[i];
    }
    p->p_name[i] = '\0';
}

void 
thread_init(struct thread *td,struct proc *p, int pri, char *name)
{
    long i;
    bzero(td,sizeof(struct thread));
    td->td_proc = p;
    td->td_state = TDS_NEW;
    td->td_mtx = MTX_INITIALIZER;
    td->td_sigmask  = 0;
    td->td_tid = get_vaild_tid();       // undefined
    td->td_factor = 128;    // undefined
    td->td_retcode = 0;
    td->td_priority = pri;
    LIST_ENTRY_INIT(td,td_runq);
    LIST_ENTRY_INIT(td,td_sleepq);
    SLIST_HEAD_INIT(td,td_pendings);
    pcb_init(); // undefined
    for(i = 0;i<MAX_NAME && name[i];i++) {
        td->td_name[i] = name[i];
    }
    td->td_name[i] = '\0';
}

void 
proc_create(struct proc *p , int cpuid , int flags , int pri,
                int multithread,char *name)
{
    struct proc *new;
    struct thread *new_td;
    struct pcpu *pcpu = PCPU_GET(cpuid);
    struct proc *tmp;

    new_td = uma_alloc(sizeof(struct thread));
    new = uma_alloc(sizeof(struct proc));
    proc_init(new,new_td,p,flags,pri,p->p_pid,cpuid,name);
    thread_init(new_td,new,pri,name);

    pri = pri == SET_DEFAULT ? DEFAULT_PRIORITY : pri;
    sched_insert(new_td,pri,cpuid,new_td->td_state);

    new->p_list.le_prev = NULL;
    LIST_INSERT_HEAD((&pcpu->pcpu_list),new,p_list);
}

int
proc_exit(struct proc *p)
{
    int ret = p->p_rthread->td_retcode;
    struct thread *td;
    // release resources
    // disconnect from a list
    // free structures
    return (ret)
}

int
proc_kill(pid_t pid)
{
    int ret;
    struct proc *p;
    if(curproc->p_pid == pid) {
        ret = proc_exit(curproc);
        return (ret);
    }


    for(p = curproc;p;p = LIST_NEXT(p,p_list)) {
        if(p->p_pid == pid) {
            ret = proc_exit(p);
            return (ret);
        }
    }

    for(p = curproc;p;p = LIST_PREV(p,p_list)) {
        if(p->p_pid == pid) {
            ret = proc_exit(p);
            return (ret);
        }
    }
}

