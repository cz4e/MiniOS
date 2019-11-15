#ifndef _MIPS_ASM_H_
#define _MIPS_ASM_H_

#if defined _MIPS_SZPTR32_
#define PTR_ADD         add
#define PTR_ADDI        addi
#define PTR_ADDU        addu
#define PTR_ADDIU       addiu
#define PTR_SUB         sub
#define PTR_SUBI        subi
#define PTR_SUBU        subu
#define PTR_SUBIU       subu
#define PTR_L           lw
#define PTR_LA          la
#define PTR_LI          li
#define PTR_S           sw
#define PTR_SLL         sll
#define PTR_SLLV        sllv
#define PTR_SRL         srl
#define PTR_SRLV        srlv
#define PTR_SRA         sra
#define PTR_SRAV        srav
#define PTR_LL          ll
#define PTR_SC          sc
#define PTR_WORD        .word
#define PTR_SCALESHIFT  2
#elif defined _MIPS_SZPTR32_
#define PTR_ADD         dadd
#define PTR_ADDI        daddi
#define PTR_ADDU        daddu
#define PTR_ADDIU       daddiu
#define PTR_SUB         dadd
#define PTR_SUBI        dsubi
#define PTR_SUBU        dsubu
#define PTR_SUBIU       dsubu
#define PTR_L           ld
#define PTR_LA          dla
#define PTR_LI          dli
#define PTR_S           sd
#define PTR_SLL         dsll
#define PTR_SLLV        dsllv
#define PTR_SRL         dsrl
#define PTR_SRLV        dsrlv
#define PTR_SRA         dsra
#define PTR_SRAV        dsrav
#define PTR_LL          lld
#define PTR_SC          scd
#define PTR_WORD        .dword
#define PTR_SCALESHIFT  3
#endif

#define _C_LABEL(x)         x

#define SZREG               8
#define CALLFRAME_SIZ       (SZREG * 4)
#define CALLFRAME_RA        (CALLFRAME_SIZ - 1 * SZREG)
#define CALLFRAME_SP        (CALLFRAME_SIZ - 2 * SZREG)

#define NESTED(x, fsize, retpc)                     \
    .globl  _C_LABEL(x);                            \
    .ent    _C_LABEL(x),0;                          \
_C_LABEL(x):    ;                                   \
    .frame sp, fsize, retpc;                        \

#define LEAF(x)                                     \
    .globl  _C_LABEL(x);                            \
    .ent    _C_LABEL(x), 0;                         \
_C_LABEL(x):    ;                                   \
    .frame sp, 0, ra;

#define END(x)                                      \
    .end    _C_LABEL(x)


#define ITLBNOPFIX  nop;nop;nop;nop;nop;nop;nop;nop;nop;sll $0,$0,3;

#define GET_CPU_PCPU(reg)                           \
    PTR_L  reg, _C_TABEL(pcpup);

#endif