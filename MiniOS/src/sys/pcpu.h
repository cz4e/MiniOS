#ifndef _SYS_PCPU_H_
#define _SYS_PCPU_H_

#include <sys/types.h>
#include <sys/queue.h>

#define PCPU_GET(cpuid)         (&(cpu[cpuid]))
#define MAX_GROUPS              64
#define PRIORITY_STEP           4
#define MAX_CPU                 256

struct thread;
struct proc;


struct pcpu {
    struct mtx          pc_lock;
    LIST_HEAD(,thread)  pc_runq[MAX_GROUPS - 1];
    LIST_HEAD(,thread)  pc_sleepq[MAX_GROUPS - 1];
    LIST_HEAD(,proc)    pc_list;
    struct thread *     pc_curthread;
    struct proc *       pc_curproc;
    struct pcpu *       pc_curpcb;
    uint32_t            pc_cpuid;
};

#define CURPCPU_GET(element)       (curpcpu->pc_##element)

#endif