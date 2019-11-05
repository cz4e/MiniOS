#ifndef _MIPS_CPUFUN_H_
#define _MIPS_CPUFUN_H_

#include <sys/cdefs.h>

static void
mips_barrier(void)                                      
{                                                       
    __asm__ volatile(".set noreorder\n\t"
			  "nop\n\t"                                 
			  "nop\n\t"
			  "nop\n\t"
			  "nop\n\t"
			  "nop\n\t"
			  "nop\n\t"
			  "nop\n\t"
			  "nop\n\t"
			  ".set reorder\n\t"
			  : : : "memory"    
    );                                                  
}
//#if defined __MIPS32__ || defined __MIPS64__
#define MIPS_RW64(n,r)                                  \
static uint64_t                                         \
mips_rd_##n(void)                                       \
{                                                       \
    int v;                                              \
    __asm__ volatile(                                   \
        "dmfc0  %[v], $"__XSTRING(r)";"                 \
        :[v] "=&r" (v)                                  \
    );                                                  \
    mips_barrier();                                     \
    return (v);                                         \
}                                                       \
                                                        \
static uint64_t                                         \
mips_wr_##n(uint64_t a)                                 \
{                                                       \
    __asm__ volatile(                                   \
        "dmtc0  %[a]","$"__XSTRING(r)";"                \
        __XSTRING()";"                                  \
        "nop;\n\t"                                      \
        "nop;\n\t"                                      \
        :                                               \
        :[a] "r"(a)                                     \
    );                                                  \
    mips_barrier();                                     \
}

#endif