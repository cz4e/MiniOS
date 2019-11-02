#ifndef _SYS_SCHED_H_
#define _SYS_SCHED_H_

#include <sys/proc.h>
#define SCHED_RUNQ              1
#define SCHED_SLEEPQ            2

void sched_insert(struct thread *td,int pri,int cpuid,flag_t flags);
struct thread *sched_choose(int cpuid,int queue,int pri);
void sched_switch(struct thread *td);
#endif