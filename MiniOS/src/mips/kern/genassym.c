#include <sys/assym.h>
#include <mips/include/pcb.h>

#define offsetof(t,m)       (int)((& ((t *) 0L)->m))

ASSYM(U_PCB_REGS,offsetof(struct pcb, pcb_regs.zero));
ASSYM(U_PCB_CONTEXT,offsetof(struct pcb, pcb_context));
ASSYM(U_PCB_FPREGS, offsetof(struct pcb, pcb_regs.f0));
ASSYM(U_PCB_ONFAULT, offsetof(struct pcb, pcb_onfault));

ASSYM(PC_CURPCB, offsetof(struct pcpu, pc_curpcb));
ASSYM(PC_SEGBASE, offsetof(struct pcpu, pc_segbase));
ASSYM(PC_CURTHREAD, offsetof(struct pcpu, pc_curthread));
ASSYM(PC_FPCURTHREAD, offsetof(struct pcpu, pc_fpcurthread));
ASSYM(PC_CPUID, offsetof(struct pcpu, pc_cpuid));
