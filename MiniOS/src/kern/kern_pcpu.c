#include <sys/pcpu.h>
#include <bzero.h>

struct pcpu cpu_pcpu[MAX_CPU];

void
pcpu_startup(void)
{
    bzero(cpu_pcpu, sizeof(struct pcpu) * MAX_CPU);

    for(int i = 0 ; i < MAX_CPU; ++i) {
        cpu_pcpu[i].pc_cpuid = i;
    }
}

void
pcpu_setcurthread(int cpuid, struct thread *curthread)
{
    if(cpuid < 0 || cpuid > MAX_CPU) return;
    PCPU_LOCK(&cpu_pcpu[cpuid].pc_lock);
    cpu_pcpu[cpuid].pc_curthread = curthread;
    PCPU_UNLOCK(&cpu_pcpu[cpuid].pc_lock);
}

void
pcpu_setcurproc(int cpuid, struct proc *curproc)
{
    if(cpuid < 0 || cpuid > MAX_CPU) return ;
    PCPU_LOCK(&cpu_pcpu[cpuid].pc_lock);
    cpu_pcpu[cpuid].pc_curproc = curproc;
    PCPU_UNLOCK(&cpu_pcpu[cpuid].pc_lock);
}

void
pcpu_setcurpcb(int cpuid, struct pcb *curpcb)
{
    if(cpuid < 0 || cpuid > MAX_CPU) return ;
    PCPU_LOCK(&cpu_pcpu[cpuid].pc_lock);
    cpu_pcpu[cpuid].pc_curpcb = curpcb;
    PCPU_UNLOCK(&cpu_pcpu[cpuid].pc_lock);
}

void
pcpu_insert_td_2runq(struct thread *td)
{
    int cpuid = td->td_cpuid;
    
    PCPU_LOCK(&cpu_pcpu[cpuid].pc_lock);
    
    PCPU_UNLOCK(&cpu_pcpu[cpuid].pc_lock);

}