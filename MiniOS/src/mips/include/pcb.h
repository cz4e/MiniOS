#ifndef _MIPS_PCB_H_
#define _MIPS_PCB_H_

#include <mips/include/cpuregs.h>

extern struct pcb *curpcb;

struct pcb {
    struct cpu_regs         pcb_regs;
    __register_t            pcb_context[14];
    void                    *pcb_onfault;
    __register_t            pcb_tpc;
};

void pcb_init(struct pcb *pcb, void  *onfaults);

#define PCB_REG_S0                      0
#define PCB_REG_S1                      1
#define PCB_REG_S2                      2
#define PCB_REG_S3                      3
#define PCB_REG_S4                      4
#define PCB_REG_S5                      5
#define PCB_REG_S6                      6
#define PCB_REG_S7                      7
#define PCB_REG_SP                      8
#define PCB_REG_S8                      9
#define PCB_REG_RA                      10
#define PCB_REG_SR                      11
#define PCB_REG_GP                      12
#define PCB_REG_PC                      13

#define SAVE_U_PCB_REG(reg, offs, base)                             \
    REG_S   reg, U_PCB_REGS + (SZREG * offs) (base)

#define RESTORE_U_PCB_REG(reg, offs, base)                          \
    REG_L   reg, U_PCB_REGS + (SZREG * offs) (base)

#define SAVE_U_PCB_FPREG(reg, offs, base)                           \
    FP_S    reg, U_PCB_FPREGS + (SZFPREG * offs) (base)

#define RESTORE_U_FPREG(reg, offs, base)                            \
    FP_L    reg, U_PCB_FPREGS + (SZFPREG * offs) (base)

#define SAVE_U_PCB_FPSR(reg, offs, base)                            \
    FP_S    reg, U_PCB_FPREGS + (SZFPREG * offs) (base)

#define RESTORE_U_PCB_FPSR(reg, offs, base)                         \
    FP_L    reg, U_PCB_FPREGS + (SZFPREG * offs) (base)

#define SAVE_U_PCB_CONTEXT(reg, offs, base)                         \
    REG_S   reg, U_PCB_CONTEXT + (SZREG * offs ) (base)

#define RESTORE_U_PCB_CONTEXT(reg, offs, base)                      \
    REG_L   reg, U_PCB_CONTXT + (SZREG * offs) (base)

#endif