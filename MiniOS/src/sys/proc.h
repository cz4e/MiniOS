#ifndef _SYS_PROC_H_
#define _SYS_PROC_H_

#include <sys/types.h>
#include <sys/queue.h>
#include <sys/signal.h>
#include <sys/mtx.h>
#include <vm/vm.h>
#include <sys/timevar.h>


#if     defined     __MIPS__
#include <mips/include/pcb.h>
#elif   defined     __X86__
#include <x86/include/pcb.h>
#endif

#define DEFAULT_PRIORITY            100
#define SET_DEFAULT                 -1
#define PID_MAX                     99999
#define NO_PID                      (PID_MAX + 1)
#define TID_MAX                     PID_MAX
#define NO_TID                      NO_PID
#define MAX_NAME                    256
struct proc;


#define TDS_NEW                     0x00000001
#define TDS_RUNNING                 0x00000002
#define TDS_SLEEPING                0x00000004
#define TDS_READY                   0x00000008
#define TDS_TERMINATED              0x00000010
#define TDS_RESCHUDLE               0x00000020

#define TDF_

struct thread {
    LIST_ENTRY(thread)  td_list;                    /* All threads                  */
    LIST_ENTRY(thread)  td_runq;                    /* Running queue                */
    LIST_ENTRY(thread)  td_sleepq;                  /* sleeping queue               */
    struct proc *       td_proc;                    /*                              */
    struct mtx          td_mtx;                     /* Mutual exclusion             */
    vm_addr_t           td_addr;                    /* Thread's base address        */
    size_t              td_size;                    /* The size of memory space     */
    tid_t               td_tid;                     /*                              */
    int                 td_state;                   /* Thread's state               */
    struct pcb          td_pcb;                     /* Process control block        */
    cpuid_t             td_cpuid;                   /* CPU which thread runs on     */
    SLIST_HEAD(,sigevent)   td_pendings;            /* signals pending queue        */
    sigmask_t           td_sigmask;                 /* signal mask                  */
    struct timevar      td_sleeptime;               /* the time of sleeping queue   */
    struct timevar      td_runtime;                 /* the running time             */
    int                 td_factor;                  /* for interactice process      */
    int                 td_retcode;                 /* return code                  */
    int                 td_priority;                /* Thread's priority            */
    int                 td_cpuid;                   /* cpu which thread run         */
    char                td_name[MAX_NAME - 1];      /* Thread's name                */
};



struct proc {
    LIST_HEAD(,thread)  p_threads;                  /* Proc's all threads           */
    LIST_ENTRY(proc)    p_list;                     /* All processes                */
    LIST_ENTRY(proc)    p_siblings;                 /* Proc's siblings              */
    LIST_HEAD(,proc)    p_childrens;                /* Proc's childrens             */
    struct proc *       p_parent;                   /* Proc's parent                */
    struct thread *     p_rthread;                  /* runing thread                */
    pid_t               p_pid;                      /* Proc's id                    */
    pid_t               p_ppid;                     /* Parent's id                  */
    int                 p_cpuid                     /* CPU which proc run on        */
    SLIST_HEAD(,sigevent)   p_pendings;
    struct mtx          p_mtx;                      /* Mutual exclusion             */
    flag_t              p_flags;                    /* Proc's flags                 */
    int                 p_priority;                 /* Proc's priority              */
    int                 p_states;                   /* Proc's states                */
    struct vmspace      p_vmspace;                  /* Proc's memeory space         */    
    char                p_name[MAX_NAME - 1];       /* Proc's name                  */
};

#define PS_NEW                      1
#define PS_READY                    2
#define PS_SLEEPING                 3
#define PS_RUNNING                  4
#define PS_TERMINATED               5
#define PS_RESCHEDULE               6

#define PF_

// declare functions
#endif