#include <sys/proc.h>
#include <include/bzero.h>
#include <sys/uma.h>
#include <sys/pcpu.h>
#include <sys/bitstring.h>



#ifdef __SMP__
struct proc *curproc[MAX_CPU];
struct thread *curthread[MAX_CPU];
#else
struct proc *curproc;
struct thread *curthread;
#endif

static struct proc *proc_idle;

bitstr_t    bit_decl(proc_pidmap,PID_MAX);              /* Proc's pid bit map        */
bitstr_t    bit_decl(thread_tidmap,PID_MAX);            /* Thread's pid bit map      */
static bitstr_t *proc_id_array[] = {
    proc_pidmap,
};
static bitstr_t *thread_id_array[] = {
    thread_tidmap,
};
#define PID_ALLOCATE_FAIL           0
#define PID_ALLOCATE_SUCCESS        1
#define TID_ALLOCATE_FIAL           0
#define TID_ALLOCATE_SUCCESS        1

#define PROC_PID                    0
#define THREAD_TID                  0

static struct mtx proc_id_lock;
static struct mtx thread_id_lock;



static int
proc_id_set(int type,pid_t id)
{
    int ret = PID_ALLOCATE_SUCCESS;
    mtx_lock(&proc_id_lock);
    if(bit_test(proc_id_array[type],id) == 0) {
#ifdef __KERNEL__
        kernel_error2("type: %d id: %d already used!\n",id,type);
#endif
        ret = PID_ALLOCATE_FAIL;
    } else {
        bit_set(proc_id_array[type],id);
    }
    mtx_unlock(&proc_id_lock);

    return (ret);
}

static void
proc_id_clear(int type,pid_t id)
{
    mtx_lock(&proc_id_lock);
    bit_clear(proc_id_array[type],id);
    mtx_unlock(&proc_id_lock);
}


static pid_t
get_vaild_pid(pid_t pid)
{
    pid_t i = pid + 1;
    
    while(i != pid) {
        if(i == NO_PID) {
            i = 0;
        }
        if(proc_id_set(PROC_PID,i) == PID_ALLOCATE_SUCCESS) {
            return (i);
        }
        i++;
    }
#ifdef __KERNEL__
    kernel_error("no free pid\n");
#endif
}

static int 
thread_id_set(int type,tid_t id)
{
    int ret = TID_ALLOCATE_SUCCESS;
    mtx_lock(&thread_id_lock);
    if(bit_test(thread_id_array[type],id) == 0) {
#ifdef __KERNEL__
        kernel_error2("tid type: %d id %d already used!\n",type,id);
#endif
        ret = TID_ALLOCATE_FIAL;
    } else {
        bit_set(thread_id_array[type],id);
    }
    mtx_unlock(&thread_id_lock);
    return (ret);
}

static void
thread_id_clear(int type,tid_t id)
{
    mtx_lock(&thread_id_lock);
    bit_clear(thread_id_array[type],id);
    mtx_unlock(&thread_id_lock);
}

static tid_t 
get_vaild_tid(tid_t id)
{
    tid_t i = id + 1;
    
    while(i != id) {
        if(i == NO_TID) {
            i = 0;
        }
        if(thread_id_set(THREAD_TID,i) == TID_ALLOCATE_SUCCESS) {
            return (i);
        }
        i++;
    }
#ifdef __KERNEL__
    kernel_error("no free tid\n");
#endif
}

void
proc_insert(struct proc *p,int cpuid)
{
    struct pcpu *pcpu = PCPU_GET(cpuid);
    LIST_INSERT_HEAD(&(pcpu->pcpu_list),p,p_list);
}

void 
proc_init(struct proc *p, struct thread *td, struct proc *parent,
             pid_t ppid, flag_t flags, int pri,int cpuid,const char *name)
{
    long i;
    bzero(p,sizeof(struct proc));
    p->p_threads.lh_first = td;
    p->p_parent = parent;
    p->p_pid = get_vaild_pid(ppid);
    p->p_ppid = ppid;
    p->p_flags = flags;
    p->p_priority = pri == SET_DEFAULT ? DEFAULT_PRIORITY : pri;
    p->p_states |= PS_NEW;
    spin_lock_init(&p->p_mtx.lock_object,&p->p_mtx.lock_class,"structure proc spin lock",LOCK_FREE); 
    LIST_HEAD_INIT(p,p_childrens);
    proc_insert(p,cpuid);

    for(i = 0;i < MAX_NAME && name[i];i++) {
        p->p_name[i] = name[i];
    }
    p->p_name[i] = '\0';
}

void 
thread_init(struct thread *td,struct proc *p, int pri, const char *name)
{
    long i;
    bzero(td,sizeof(struct thread));
    td->td_proc = p;
    td->td_state |= TDS_NEW;
    spin_lock_init(&td->td_mtx.lock_object,&td->td_mtx.lock_class,"structure thread spin lock",LOCK_FREE);  
    td->td_sigmask  = 0;
    td->td_tid = get_vaild_tid(p->p_pid);
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
    struct proc *oldp;
    struct pcpu *pcpu = PCPU_GET(p->p_cpuid);
    
    for(oldp = LIST_FIRST(&(pcpu->pcpu_list));
        oldp != p;
        oldp = LIST_NEXT(oldp,p_list));
    
    if(oldp->p_list.le_prev == NULL) {
        LIST_REMOVE_HEAD(&(pcpu->pcpu_list));
    } else {
        LIST_REMOVE(oldp,p_list);
    }

    LIST_FOREACH(td,&(p->p_threads),td_list) {
        
        ret = thread_exit(td);
    }
    kmfree(p->p_vmspace.mms_start);
    kmfree(td);
    kmfree(p);
    return (ret);
}

int 
thread_exit(struct thread *td)
{
    struct pcpu *pcpu = PCPU_GET(td->td_cpuid);
    int pri = td->td_priority % PRIORITY_STEP;
    struct thread *oldtd;

    if(td->td_state & (TDS_NEW | TDS_READY | TDS_RUNNING)) {
        LIST_FOREACH(oldtd,&(pcpu->pcpu_runq[pri]),td_runq) {
            if(oldtd->td_runq.le_prev == NULL) {
                LIST_REMOVE_HEAD(&(pcpu->pcpu_runq[pri]));
            } else {
                LIST_REMOVE(oldtd,td_runq);
            }
        }
    }
    else if(td->td_state & (TDS_SLEEPING)) {
        LIST_FOREACH(oldtd,&(pcpu->pcpu_sleepq[pri]),td_sleepq) {
            if(oldtd->td_sleepq.le_prev == NULL) {
                LIST_REMOVE_HEAD(&(pcpu->pcpu_sleepq[pri]));
            } else {
                LIST_REMOVE(oldtd,td_sleepq);
            }
        }
    }
}

int
proc_kill(pid_t pid)
{
    int ret = 1;
    struct proc *p,**procp;
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

    for(procp = &curproc;procp && *procp;procp = LIST_PREV(p,p_list)) {
        if((*procp)->p_pid == pid) {
            ret = proc_exit(*procp);
            return (ret);
        }
    }

    return (ret);
}

