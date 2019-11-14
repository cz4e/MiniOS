#include <sys/proc.h>
#include <sys/mm.h>

struct proc *
kern_fork(int zone ,struct proc *parent,const char *name)
{
    struct proc* new_proc;
    struct thread *new_thread;

    new_proc = kmalloc(zone,sizeof(struct zone),M_NOWAIT);
    new_thread = kmalloc(zone,sizeof(struct zone),M_NOWAIT);
    /* Start initial a process structure and thread structure */
    PROC_LOCK(&parent->p_mtx);
    proc_init(new_proc, new_thread, parent, 
                            parent->p_pid,parent->p_flags, 
                                parent->p_priority,parent->p_cpuid, name);
    thread_init(new_thread, new_proc, parent->p_priority,name);
    new_proc->p_states = (new_proc->p_states & ~PS_NEW )| PS_READY;
    new_thread->td_state = (new_thread->td_state & ~TDS_NEW) | TDS_READY;
    PROC_UNLOCK(&parent->p_mtx);

    sched_insert(new_thread,new_thread->td_priority,new_thread->td_cpuid,new_thread->td_state);
    new_proc->p_list.le_prev =  NULL;
    LIST_INSERT_HEAD(&pcpu->pcpu_list,new_proc,p_list);

    /* Here, structure initial done */
    return (new_proc);
}

int
kern_exit(struct proc *p)
{
    int ret = proc_exit(p);
    return (ret);
}

int 
kern_kill(pid_t pid)
{
    int ret;

    ret = proc_kill(pid);

    return (ret);
}

