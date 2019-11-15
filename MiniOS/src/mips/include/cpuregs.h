#ifndef _MIPS_CPUREGS_H_
#define _MIPS_CPUREGS_H_

#define _(n)            n

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
#define MIPS_COP_0_TLB_ENTRYLO1_SELECT _(1)
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
#define MIPS_COP_0_TAGLO_SELECT     _(0)
#define MIPS_COP_0_DATALO           _(28)
#define MIPS_COP_0_DATALO_SELECT    _(1)
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


/* Status register flags */
#define SR_IE_OFFSET                0
#define SR_EXL_OFFSET               1
#define SR_ERL_OFFSET               2
#define SR_KSU_OFFSET               3
#define SR_UX_OFFSET                5
#define SR_SX_OFFSET                6
#define SR_KX_OFFSET                7
#define SR_IM_OFFSET                8
#define SR_IPL_OFFSET               10
#define SR_NMI_OFFSET               19
#define SR_SR_OFFSET                20
#define SR_TS_OFFSET                21
#define SR_BEV_OFFSET               22
#define SR_PX_OFFSET                23
#define SR_MX_OFFSET                24
#define SR_RE_OFFSET                25
#define SR_FR_OFFSET                26
#define SR_RP_OFFSET                27
#define SR_CU_OFFSET                28

#define MIPS_SR_IE                  (0x1 << SR_IE_OFFSET)
#define MIPS_SR_EXL                 (0x1 << SR_EXL_OFFSET)
#define MIPS_SR_ERL                 (0x1 << SR_ERL_OFFSET)
#define MIPS_SR_KSU                 (0x3 << SR_KSU_OFFSET)
#define MIPS_SR_UX                  (0x1 << SR_UX_OFFSET)
#define MIPS_SR_SX                  (0x1 << SR_SX_OFFSET)
#define MIPS_SR_KX                  (0x1 << SR_KX_OFFSET)
#define MIPS_SR_IM                  (0x3 << SR_IM_OFFSET)
#define MIPS_SR_IPL                 (0x3f << SR_IPL_OFFSET)
#define MIPS_SR_NMI                 (0x1 << SR_NMI_OFFSET)
#define MIPS_SR_SR                  (0x1 << SR_SR_OFFSET)
#define MIPS_SR_TS                  (0x1 << SR_TS_OFFSET)
#define MIPS_SR_BEV                 (0x1 << SR_BEV_OFFSET)
#define MIPS_SR_PX                  (0x1 << SR_PX_OFFSET)
#define MIPS_SR_MX                  (0x1 << SR_MX_OFFSET)
#define MIPS_SR_RE                  (0x1 << SR_RE_OFFSET)
#define MIPS_SR_FR                  (0x1 << SR_FR_OFFSET)
#define MIPS_SR_RP                  (0x1 << SR_RP_OFFSET)
#define MIPS_SR_CU                  (0xf << SR_CU_OFFSET)


#define CAUSE_EXCCODE_OFFSET        2
#define CAUSE_IP_FIELD0_OFFSET      8
#define CAUSE_IP_FIELD1_OFFSET      10
#define CAUSE_RIPL_OFFSET           CAUSE_IP_FIELD1_OFFSET
#define CAUSE_WP_OFFSET             22
#define CAUSE_IV_OFFSET             23
#define CAUSE_PCI_OFFSET            26
#define CAUSE_DC_OFFSET             27
#define CAUSE_CE_OFFSET             28
#define CAUSE_TI_OFFSET             30
#define CAUSE_BD_OFFSET             31

#define MIPS_CAUSE_EXECCODE         (0x1 << CAUSE_EXCCODE_OFFSET)
#define MIPS_CAUSE_IP_FIELD0        (0x3 << CAUSE_IP_FIELD0_OFFSET)
#define MIPS_CAUSE_IP_FILED1        (0x3f << CAUSE_IP_FIELD1_OFFSET)
#define MIPS_CAUSE_RIPL             (0x3f << CAUSE_RIPL_OFFSET)
#define MIPS_CAUSE_WP               (0x1 << CAUSE_WP_OFFSET)
#define MIPS_CAUSE_IV               (0x1 << CAUSE_IV_OFFSET)
#define MIPS_CAUSE_PCI              (0x1 << CAUSE_PCI_OFFSET)
#define MIPS_CAUSE_DC               (0x1 << CAUSE_DC_OFFSET)
#define MIPS_CAUSE_CE               (0x3 << CAUSE_CE_OFFSET)
#define MIPS_CAUSE_TI               (0x1 << CAUSE_TI_OFFSET)
#define MIPS_CAUSE_BD               (0x1 << CAUSE_BD_OFFSET)

#define CAUSE_EXECCODE_INT                0
#define CAUSE_EXECCODE_MOD                1
#define CAUSE_EXECCODE_TLBL               2
#define CAUSE_EXECCODE_TLBS               3
#define CAUSE_EXECCODE_ADEL               4
#define CAUSE_EXECCODE_ADES               5
#define CAUSE_EXECCODE_IBE                6
#define CAUSE_EXECCODE_DBE                7
#define CAUSE_EXECCODE_SYSCALL            8
#define CAUSE_EXECCODE_BP                 9
#define CAUSE_EXECCODE_RI                 10
#define CAUSE_EXECCODE_CPU                11
#define CAUSE_EXECCODE_OV                 12
#define CAUSE_EXECCODE_TRAP               13
#define CAUSE_EXECCODE_UNUSED             14
#define CAUSE_EXECCODE_FPE                15
#define CAUSE_EXECCODE_C2E                18
#define CAUSE_EXECCODE_MDMX               22
#define CAUSE_EXECCODE_WATCH              23
#define CAUSE_EXECCODE_MCHECK             24
#define CAUSE_EXECCODE_THREAD             25
#define CAUSE_EXECCODE_DSP                26
#define CAUSE_EXECCODE_CACHEERR           30

#define MIPS_COP_0_TLB_INDEX                _(0)
#define MIPS_COP_0_TLB_RANDOM               _(1)
#define MIPS_COP_0_TLB_CONTEXT              _(4)
#define MIPS_COP_0_BAD_VADDR                _(8)
#define	MIPS_COP_0_TLB_HI	                _(10)
#define	MIPS_COP_0_STATUS	                _(12)
#define	MIPS_COP_0_CAUSE	                _(13)
#define	MIPS_COP_0_EXC_PC	                _(14)
#define	MIPS_COP_0_PRID		                _(15)

struct cpu_regs {
    __register_t    zero;
    __register_t    ast;
    __register_t    v0;
    __register_t    v1;
    __register_t    a0;
    __register_t    a1;
    __register_t    a2;
    __register_t    a3;

#if defined(_MIPS_32_) || defined(_MIPS_64_)
    __register_t    a4;
    __register_t    a5;
    __register_t    a6;
    __register_t    a7;
    __register_t    t0;
    __register_t    t1;
    __register_t    t2;
    __register_t    t3;
#else
    __register_t    t0;
    __register_t    t1;
    __register_t    t2;
    __register_t    t3;
    __register_t    t4;
    __register_t    t5;
    __register_t    t6;
    __register_t    t7;
#endif
    __register_t    s0;
    __register_t    s1;
    __register_t    s2;
    __register_t    s3;
    __register_t    s4;
    __register_t    s5;
    __register_t    s6;
    __register_t    s7;
    __register_t    s8;

    __register_t    t8;
    __register_t    t9;

    __register_t    k0;
    __register_t    k1;

    __register_t    gp;
    __register_t    sp;
    __register_t    ra;
    __register_t    sr;
    __register_t    mullo;
    __register_t    mulhi;
    __register_t    badvaddr;
    __register_t    cause;
    __register_t    pc;

    __register_t    ic;
    __register_t    unused;

    __register_t    f0;
    __register_t    f1;
    __register_t    f2;
    __register_t    f3;
    __register_t    f4;
    __register_t    f5;
    __register_t    f6;
    __register_t    f7;
    __register_t    f8;
    __register_t    f9;
    __register_t    f10;
    __register_t    f11;
    __register_t    f12;
    __register_t    f13;
    __register_t    f14;
    __register_t    f15;
    __register_t    f16;
    __register_t    f17;
    __register_t    f18;
    __register_t    f19;
    __register_t    f20;
    __register_t    f21;
    __register_t    f22;
    __register_t    f23;
    __register_t    f24;
    __register_t    f25;
    __register_t    f26;
    __register_t    f27;
    __register_t    f28;
    __register_t    f29;
    __register_t    f30;
    __register_t    f31;
    __register_t    fsr;
    __register_t    fir;
};

#endif