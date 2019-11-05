# void _pcb_regs_save(struct cpu_regs *cs)

#include <mips/include/pcb.h>
#include <mips/include/cpu.h>

.section        .text
.globl  _pcb_regs_save
.type   _pcb_regs_save, @function
_pcb_regs_save:
