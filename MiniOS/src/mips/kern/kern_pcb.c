#include <mips/include/pcb.h>

extern void _pcb_regs_init(struct cpuset *);

void
pcb_init(struct pcb *pcb,void *onfault)
{
    _pcb_regs_init(&(pcb->pcb_cpuset));
    pcb->cs_onfault = onfault;
}