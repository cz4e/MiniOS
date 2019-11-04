#ifndef _MIPS_PCB_H_
#define _MIPS_PCB_H_

#include <mips/include/cpuset.h>

struct pcb {
    struct cpuset       pcb_cpuset;
    __register_t        cs_context[14];
    void               *cs_onfault;
    __register_t        cs_tpc;
};

void pcb_init(struct pcb *pcb, void  *onfaults);
#endif