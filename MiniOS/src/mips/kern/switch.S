#include <mips/include/pcb.h>

/* savectx(struct pcb *pcb)*/
LEAF(savectx)
    SAVE_U_PCB_CONTEXT(s0, PCB_REG_S0, a0)
    SAVE_U_PCB_CONTEXT(s1, PCB_REG_S1, a0)
    SAVE_U_PCB_CONTEXT(s2, PCB_REG_S2, a0)
    SAVE_U_PCB_CONTEXT(s3, PCB_REG_S3, a0)
    mfc0    v0, MIPS_COP_0_STATUS
    SAVE_U_PCB_CONTEXT(s4, PCB_REG_S4, a0)
    SAVE_U_PCB_CONTEXT(s5, PCB_REG_S5, a0)
    SAVE_U_PCB_CONTEXT(s6, PCB_REG_S6, a0)
    SAVE_U_PCB_CONTEXT(s7, PCB_REG_S7, a0)
    SAVE_U_PCB_CONTEXT(sp, PCB_REG_SP, a0)
    SAVE_U_PCB_CONTEXT(s8, PCB_REG_S8, a0)
    SAVE_U_PCB_CONTEXT(ra, PCB_REG_SA, a0)
    SAVE_U_PCB_CONTEXT(v0, PCB_REG_SR, a0)
    SAVE_U_PCB_CONTEXT(gp, PCB_REG_GP, a0)

    move    v0, ra
    jal     1f
    nop
1:
    SAVE_U_PCB_CONTEXT(ra, PCB_REG_PC, a0)
    move    ra, v0

    j ra
    move v0, zero
END(savectx)


/*  a0, old
    a1, new
    a2, mtx
*/

NESTED(cpu_switch, CALLFRAME_SIZ, ra)
    mfc0    t0, MIPS_COP_0_STATUS
    nop
    nop
    /* disable inturrupt */
    and     a3, t0, ~(MIPS_SR_INT_IE)
    mtc0    a3, MIPS_COP_0_STATUS   
    ITLBNOPFIX
    beqz    a0, mips_sw1
    move    a3, a0
    PTR_L   a0, TD_PCB(a0)
    SAVE_U_PCB_CONTEXT(sp, PCB_REG_SP, a0)
    PTR_SUBU    sp, sp, CALLFRAME_SIZ
    REG_S   ra, CALLFRAME_RA(sp)
    .mask   0x80000000, (CALLFRAME_RA - CALLFRAME_SIZ)
    SAVE_U_PCB_CONTEXT(s0, PCB_REG_S0, a0)
    SAVE_U_PCB_CONTEXT(s1, PCB_REG_S1, a0)
    SAVE_U_PCB_CONTEXT(s2, PCB_REG_S2, a0)
    SAVE_U_PCB_CONTEXT(s3, PCB_REG_S3, a0)
    SAVE_U_PCB_CONTEXT(s4, PCB_REG_S4, a0)
    SAVE_U_PCB_CONTEXT(s5, PCB_REG_S5, a0)
    SAVE_U_PCB_CONTEXT(s6, PCB_REG_S6, a0)
    SAVE_U_PCB_CONTEXT(s7, PCB_REG_S7, a0)
    SAVE_U_PCB_CONTEXT(s8, PCB_REG_S8, a0)
    SAVE_U_PCB_CONTEXT(ra, PCB_REG_RA, a0)
    SAVE_U_PCB_CONTEXT(t0, PCB_REG_SR, a0)
    SAVE_U_PCB_CONTEXT(gp, PCB_REG_GP, a0)
    jal getpc
    nop
getpc:
    SAVE_U_PCB_CONTEXT(ra, PCB_REG_PC, a0)

mips_sw1:
    move    s7, a1
    GET_CPU_PCPU(a3)
    PTR_S   a1, PC_CURTHREAD(a3)
    PTR_L   a2, TD_PCB(a1)
    PTR_S   a2, PC_CURPCB(a3)
    PTR_L   v0, TD_KSTACK(a1)
    PTR_L   s0, MIPS_KSEG2_START


    PTR_L   s0, TD_PCB(s7)
    RESTORE_U_PCB_CONTEXT(sp, PCB_REG_SP, s0)
//    PTL_LA  t1, _C_LABLE(pmap_activate)
//    jalr    t1
//    move    a0, s7

    move    a0, s0
    move    a1, s1
    RESTORE_U_PCB_CONTEXT(gp, PCB_REG_GP, a0)
    RESTORE_U_PCB_CONTEXT(v0, PCB_REG_SR, a0)
    RESTORE_U_PCB_CONTEXT(ra, PCB_REG_RA, a0)
    RESTORE_U_PCB_CONTEXT(s0, PCB_REG_S0, a0)
    RESTORE_U_PCB_CONTEXT(s1, PCB_REG_S1, a0)
    RESTORE_U_PCB_CONTEXT(s2, PCB_REG_S2, a0)
    RESTORE_U_PCB_CONTEXT(s3, PCB_REG_S3, a0)
    RESTORE_U_PCB_CONTEXT(s4, PCB_REG_S4, a0)
    RESTORE_U_PCB_CONTEXT(s5, PCB_REG_S5, a0)
    RESTORE_U_PCB_CONTEXT(s6, PCB_REG_S6, a0)
    RESTORE_U_PCB_CONTEXT(s7, PCB_REG_S7, a0)
    RESTORE_U_PCB_CONTEXT(s8, PCB_REG_S8, a0)

    mfc0    t0, MIPS_COP_0_STATUS
    and     t0, t0, MIPS_SR_INT_IE_MASK
    and     v0, v0, ~MIPS_SR_INT_IE_MASK
    or      v0, v0, t0
    mtc0    v0, MIPS_COP_0_STATUS
    ITLBNOPFIX

END(cpu_swith)