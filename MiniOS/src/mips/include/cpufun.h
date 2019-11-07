#ifndef _MIPS_CPUFUN_H_
#define _MIPS_CPUFUN_H_

#include <sys/cdefs.h>
#include <sys/types.h>
#include <mips/include/cpuregs.h>

static void
mips_barrier(void)                                      
{    
#if defined(CPU_CNMIPS) || defined(CPU_RMI) || defined(CPU_NLM)
    __compiler_membar();
#else                                                   
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
#endif                                                
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

#define MIPS_RW32_C0(name,reg)                          \
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

#define MIPS_RW32_C0_SEL(name,reg,select)               \
static inline uint32_t                                  \
mips_read_##name(void)                                  \
{                                                       \
    int v0;                                             \
    __asm__ volatile(                                   \
        "mfc0   %[v0], $"__XSTRING(reg)","__XSTRING(select)";"  \
        :[v0] "=&r" (v0)                                \
    );                                                  \
    mips_barrier();                                     \
    return (v0);                                        \
}                                                       \
                                                        \
static inline void                                      \
mips_write_##name(uint32_t a0)                          \
{                                                       \
    __asm__ volatile(                                   \
        "mtc0   %[a0], $"__XSTRING(reg)","__XSTRING(select)";"  \
        __XSTRING(COP0_SYNC)";"                         \
        "nop;"                                          \
        "nop;"                                          \
        :                                               \
        :[a0] "r"(a0)                                   \
    );                                                  \
    mips_barrier();                                     \
}

#define MIPS_RW32_C0_SEL(name,reg,select)               \
static inline uint64_t                                  \
mips_read_##name(void)                                  \
{                                                       \
    int v0;                                             \
    __asm__ volatile(                                   \
        "dmfc0  %[v0], $"__XSTRING(reg)","__XSTRING(select)";"  \
        : [v0] "=&r" (v0)                               \
    );                                                  \
    mips_barrier();                                     \
    return (v0);                                        \
}                                                       \
                                                        \
static inline void                                      \
mips_write_##name(uint64_t a0)                          \
{                                                       \
    __asm__ volatile(                                   \
        "dmtc0 %[a0], $"__XSTRING(reg)","__XSTRING(select); \
        __XSTRING(COP0_SYNC)";"                         \
        "nop;"                                          \
        "nop;"                                          \
        :                                               \
        : [a0] "r" (a0)                                 \
    );                                                  \
}

#ifdef __MIPS32__
MIPS_RW32_C0(sr,MIPS_COP_0_SR);
MIPS_RW32_C0(cause,MIPS_COP_0_CAUSE);
MIPS_RW32_C0(epc,MIPS_COP_0_EPC);
MIPS_RW32_C0(compare,MIPS_COP_0_COMPARE);
MIPS_RW32_C0(count,MIPS_COP_0_COUNT);
MIPS_RW32_C0(badvaddr,MIPS_COP_0_BADVADDR);
MIPS_RW32_C0(tlb_context,MIPS_COP_0_TLB_CONTEXT);
MIPS_RW32_C0(tlb_entryhi,MIPS_COP_0_TLB_ENTRYHI);
MIPS_RW32_C0(tlb_entrylo0,MIPS_COP_0_TLB_ENTRYLO0)
MIPS_RW32_C0(tlb_entrylo1,MIPS_COP_0_TLB_ENTRYLO1);
MIPS_RW32_C0(tlb_index,MIPS_COP_0_TLB_INDEX);
MIPS_RW32_C0(tlb_pagemask,MIPS_COP_0_TLB_PAGEMASK);
MIPS_RW32_C0(tlb_random,MIPS_COP_0_TLB_RANDOM);
MIPS_RW32_C0(tlb_wired,MIPS_COP_0_TLB_WIRED);
MIPS_RW32_C0(prid,MIPS_COP_0_PRID);
MIPS_RW32_C0(config,MIPS_COP_0_CONFIG);
MIPS_RW32_C0_SEL(config1,MIPS_COP_0_CONFIG,MIPS_COP_0_CONFIG1_SELECT);
MIPS_RW32_C0_SEL(config2,MIPS_COP_0_CONFIG,MIPS_COP_0_CONFIG2_SELECT);
MIPS_RW32_C0_SEL(config2,MIPS_COP_0_CONFIG,MIPS_COP_0_CONFIG3_SELECT);
MIPS_RW32_C0_SEL(ebase,MIPS_COP_0_EBASE,MIPS_COP_0_EBASE_SELECT);
MIPS_RW32_C0_SEL(intctl,MIPS_COP_0_INTCTL,MIPS_COP_0_INTCTL_SELECT);
MIPS_RW32_C0_SEL(srsctl,MIPS_COP_0_SRSCTL,MIPS_COP_0_SRSCTL_SELECT);
MIPS_RW32_C0_SEL(srsmap,MIPS_COP_0_SRSMAP,MIPS_COP_0_SRSMAP_SELECT);
MIPS_RW32_C0(cacheerr,MIPS_COP_0_CACHEERR);
MIPS_RW32_C0(ecc,MIPS_COP_0_ECC);
MIPS_RW32_C0(errorepc,MIPS_COP_0_ERROREPC);
MIPS_RW32_C0_SEL(taglo,MIPS_COP_0_TAGLO,MIPS_COP_0_TAGLO_SELECT);
MIPS_RW32_C0_SEL(datalo,MIPS_COP_0_DATALO,MIPS_COP_0_DATALO_SELECT);
MIPS_RW32_C0_SEL(taghi,MIPS_COP_0_TAGHI,MIPS_COP_0_TAGHI_SELECT);
MIPS_RW32_C0_SEL(datahi,MIPS_COP_0_DATAHI,MIPS_COP_0_DATAHI_SELECT);
MIPS_RW32_C0_SEL(debug,MIPS_COP_0_DEBUG,MIPS_COP_0_DEBUG_SELECT);
MIPS_RW32_C0_SEL(depc,MIPS_COP_0_DEPC,MIPS_COP_0_DEPC_SELECT);
MIPS_RW32_C0_SEL(desave,MIPS_COP_0_DESAVE,MIPS_COP_0_DESAVE_SELECT);
MIPS_RW32_C0_SEL(watchlo,MIPS_COP_0_WATCHLO,MIPS_COP_0_WATCHLO_SELECT);
MIPS_RW32_C0_SEL(watchhi,MIPS_COP_0_WATCHHI,MIPS_COP_0_WATCHHI_SELECT);
MIPS_RW32_C0_SEL(perfctl,MIPS_COP_0_PERFCTL,MIPS_COP_0_PREFCTL_SELECT);
MIPS_RW32_C0_SEL(perfcnt,MIPS_COP_0_PERFCNT,MIPS_COP_0_PERFCNT_SELECT);
MIPS_RW32_C0_SEL(lladdr,MIPS_COP_0_LLADDR,MIPS_COP_0_LLADDR_SELECT);
MIPS_RW32_C0_SEL(hwrena,MIPS_COP_0_HWRENA,MIPS_COP_0_HWRENA_SELECT);
#endif

static inline register_t
intr_disable(void)
{
    register_t status;
    status = mips_read_sr();

    mips_write_sr(status & ~MIPS_INTR_IE);

    return (status & MIPS_INTR_IE);
}

static inline register_t 
intr_enable(void)
{
    register_t status;
    status = mips_read_sr();
    mips_write_sr(status | MIPS_INTR_IE);

    return (status);
}

#endif