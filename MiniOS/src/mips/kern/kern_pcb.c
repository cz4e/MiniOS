#include <mips/include/pcb.h>

extern void _pcb_regs_init(struct cpu_regs *);

void
pcb_init(struct pcb *pcb,void *onfault)
{
    _pcb_regs_init(&(pcb->pcb_cpuset.cs_regs));
    pcb->cs_onfault = onfault;
}