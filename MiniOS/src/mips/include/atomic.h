#ifndef _MIPS_ATOMIC_H_
#define _MIPS_ATOMIC_H_

#include <sys/types.h>

static inline void
atomic_set_int(volatile uint32_t *p,uint32_t v)
{
    uint32_t tmp;
    __asm__ volatile(
        "1:\tll  %0, %3\n\t"
        "or     %0, %2, %0\n\t"
        "sc     %0, %1\n\t"
        "beqz   %0, 1b\n\t"
        :"=&r" (tmp),"=m" (*p)
        :"r" (v),"m" (*p)
        :"memory"
    );
}

static inline void
atomic_add_int(volatile uint32_t *p,uint32_t v)
{
    uint32_t tmp;

    __asm__ volatile(
        "1:\tll %0, %3\n\t"
        "addu   %0, %2, %0\n\t"
        "sc     %0,%1\n\t"
        "beqz   %0, 1b\n\t"
        :"=&r" (tmp),"=m" (*p)
        :"r" (v), "m"(*p)
        :"memory"
    );
}

static inline void
atomic_sub_int(volatile uint32_t *p,uint32_t v)
{
    uint32_t tmp;
    __asm__ volatile(
        "1:\tll %0, %3\n\t"
        "subu   %0, %2\n\t"
        "sc     %0, %1\n\t"
        "beqz   %0, 1b\n\t"
        :"=&r" (tmp),"=m" (*p)
        :"r"(v),"m"(*p)
        :"memory"
    );
}

static int 
atomic_test_eq_int(uint32_t p,uint32_t v)
{

}

static int
atomic_test_gt_int(uint32_t p,uint32_t v)
{

}

static int
atomic_test_ge_int(uint32_t p,uint32_t v)
{

}

static int
atomic_test_lt_int(uint32_t p,uint32_t v)
{

}

static int
atomic_test_le_int(uint32_t p,uint32_t v)
{
    
}
#endif