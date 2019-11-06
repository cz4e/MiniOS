#ifndef _MIPS_CPUFUN_H_
#define _MIPS_CPUFUN_H_

#include <sys/cdefs.h>
#include <mips/include/cpuregs.h>

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

#define MIPS_RW32_C0(name,reg)                          \
static inline uint32_t                                  \
mips_read_##name(void)                                  \
{                                                       \
    int32_t v0;                                         \
                                                        \
    __asm__ volatile(                                   \
        "mfc0   %[v0], $"__XSTRING(reg)";"              \
        :[v0] "=&r"(v0)                                 \
    );                                                  \
    mips_barrier();                                     \
    return (v0);                                        \
}                                                       \
                                                        \
static inline void                                      \
mips_write_##name(uint32_t a0)                          \
{                                                       \
    __asm__ volatile(                                   \
        "mtc0   %[a0], $"__XSTRING(reg)";"              \
        __XSTRING(COP0_SYNC)";"                         \
        :[a0] "=&r" (a0)                                \
    );                                                  \
    mips_barrier();                                     \
}                                                       \
                                                        \

#define MIPS_RW64_C0(name,reg)                          \
static inline uint64_t                                  \
mips_read_##name(void)                                  \
{                                                       \
    int32_t v0;                                         \
    __asm__ volatile(                                   \
        "dmfc0 %[v0], $"__XSTRING(reg)";"               \
        :[v0] "=&r"(v0)                                 \
    );                                                  \
    mips_barrier();                                     \
    return (v0);                                        \
}                                                       \
                                                        \
static inline void                                      \
mips_write_##name(uint64_t a0)                          \
{                                                       \
    __asm__ volatile(                                   \
        "dmtc0  %[a0], $"__XSTRING(reg)";"              \
        __XSTRING(COP0_SYNC);                           \
        "nop;"                                          \
        "nop;"                                          \
        :[a0] "=&r" (a0)                                \
    );                                                  \
    mips_barrier();                                     \
}

#ifdef __MIPS64__
MIPS_RW64_C0(count,MIPS_COP_0_COUNT)
#elif defined __MIPS32__
MIPS_RW32_C0(count,MIPS_COP_0_COUNT)
#endif
#endif