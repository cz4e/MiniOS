#ifndef _MIPS_CPUREGS_H_
#define _MISP_CPUREGS_H_


#define MIPS_COP_0_SR               _(12)
#define MIPS_COP_0_CAUSE            _(13)
#define MIPS_COP_0_EPC              _(14)

#define MIPS_COP_0_COUNT            _(9)
#define MIPS_COP_0_COMPARE          _(11)

#define MIPS_COP_0_BADVADDR         _(8)

#define MIPS_COP_0_TLB_CONTEXT      _(4)
#define MIPS_COP_0_TLB_ENTRYLO0     _(2)
#define MIPS_COP_0_TLB_ENTRYLO0_SELECT _(0)
#define MIPS_COP_0_TLB_ENTRYLO1     _(3)
#define MIPS_COP_0_TLB_ENTRYLO1_SELECT _(1_)
#define MIPS_COP_0_TLB_ENTRYHI      _(10)
#define MIPS_COP_0_TLB_INDEX        _(0)
#define MIPS_COP_0_TLB_PAGEMASK     _(5)
#define MIPS_COP_0_TLB_RANDOM       _(1)
#define MIPS_COP_0_TLB_WIRED        _(6)

#define MIPS_COP_0_PRID             _(15)

#define MIPS_COP_0_CONFIG           _(16)
#define MIPS_COP_0_CONFIG1_SELECT   _(1)
#define MIPS_COP_0_CONFIG2_SELECT   _(2)
#define MIPS_COP_0_CONFIG3_SELECT   _(3)

#define MIPS_COP_0_EBASE            _(15)
#define MIPS_COP_0_EBASE_SELECT     _(1)

#define MIPS_COP_0_INTCTL           _(12)
#define MIPS_COP_0_INTCTL_SELECT    _(1)

#define MIPS_COP_0_SRSCTL           _(12)
#define MIPS_COP_0_SRSCTL_SELECT    _(2)
#define MIPS_COP_0_SRSMAP           _(12)
#define MIPS_COP_0_SRSMAP_SELECT    _(3)

#define MIPS_COP_0_CACHEERR         _(27)
#define MIPS_COP_0_ECC              _(26)
#define MIPS_COP_0_ERROREPC         _(30)

#define MIPS_COP_0_TAGLO            _(28)
#define MIPS_COP_0_TAGLO0_SELECT    _(0)
#define MIPS_COP_0_DATALO           _(28)
#define MIPS_COP_0_DATALO1_SELECT   _(1)
#define MIPS_COP_0_TAGHI            _(29)
#define MIPS_COP_0_TAGHI_SELECT     _(0)
#define MIPS_COP_0_DATAHI           _(29)
#define MIPS_COP_0_DATAHI_SELECT    _(1)

#define MIPS_COP_0_DEBUG            _(23)
#define MIPS_COP_0_DEBUG_SELECT     _(0)
#define MIPS_COP_0_DEPC             _(24)
#define MIPS_COP_0_DEPC_SELECT      _(0)
#define MIPS_COP_0_DESAVE           _(31)
#define MIPS_COP_0_DESAVE_SELECT    _(0)

#define MIPS_COP_0_WATCHLO          _(18)
#define MIPS_COP_0_WATCHLO_SELECT   _(0)
#define MIPS_COP_0_WATCHHI          _(19)
#define MIPS_COP_0_WATCHHI_SELECT   _(0)

#define MIPS_COP_0_PERFCTL          _(25)
#define MIPS_COP_0_PREFCTL_SELECT   _(0)
#define MIPS_COP_0_PERFCNT          _(25)
#define MIPS_COP_0_PERFCNT_SELECT   _(1)
#define MIPS_COP_0_LLADDR           _(17)
#define MIPS_COP_0_LLADDR_SELECT    _(0)
#define MIPS_COP_0_HWRENA           _(7)
#define MIPS_COP_0_HWRENA_SELECT    _(0)

/* machine specifical settings */
#if defined(CPU_CNMIPS) || defined (CPU_RMI)
#define COP0_SYNC
#elif defined CPU_NLM
#define COP0_SYNC   .word 0xc0
#elif defined CPU_SB1
#define COP0_SYNC   ssnop; ssnop; ssnop; ssnop; ssnop; ssnop; ssnop; ssnop; ssnop
#elif defined(CPU_MIPS24K) || defined(CPU_MIPS34K) ||		\
      defined(CPU_MIPS74K) || defined(CPU_MIPS1004K)  ||	\
      defined(CPU_MIPS1074K) || defined(CPU_INTERAPTIV) ||	\
      defined(CPU_PROAPTIV)
#define COP0_SYNC    .word 0xc0
#else
#define	COP0_SYNC  ssnop; ssnop; ssnop; ssnop; .word 0xc0;
#endif


#endif