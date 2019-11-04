#ifndef _MIPS_CPUSET_H_
#define _MIPS_CPUSET_H_

#include <mips/include/cpu.h>

struct cpuset {
    struct cpu_regs cs_regs;
};

#endif