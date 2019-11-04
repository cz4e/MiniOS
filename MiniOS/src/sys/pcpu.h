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
    LIST_HEAD(,thread)  pcpu_runq[MAX_GROUPS - 1];
    LIST_HEAD(,thread)  pcpu_sleepq[MAX_GROUPS - 1];
    LIST_HEAD(,proc)    pcpu_list;
    struct thread *     pcpu_curthread;
    struct proc *       pcpu_proc;
};

#endif