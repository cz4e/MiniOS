#ifndef _MIPS_CPU_H_
#define _MIPS_CPU_H_

#include <mips/include/_types.h>

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