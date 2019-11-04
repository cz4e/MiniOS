#ifndef _MIPS_ATOMIC_H_
#define _MIPS_ATOMIC_H_

#include <sys/types.h>

static inline void
atomic_set_int32(volatile uint32_t *p,uint32_t v)
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
atomic_clear_int32(volatile uint32_t *p,uint32_t v)
{

}

static inline void
atomic_add_int32(volatile uint32_t *p,uint32_t v)
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
atomic_sub_int32(volatile uint32_t *p,uint32_t v)
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
atomic_test_eq_int32(uint32_t p,uint32_t v)
{
    int ret;
    __asm__ volatile(
        "1:\tll %0, %3\n\t"
        "beq    %1, %2, 2f\n\t"
        "li     $0, 0\n\t"
        "sc     %0, %1\n\t"
        "beqz   %0, 1b\n\t"
        "j  3f\n\t"
        "2:\n\t"
        "li     %0, 1"
        "3:\n\t"
        :"=&r"(ret),"=m" (p)
        :"r" (v),"m" (p)
        :"memory"
    );

    return (ret);
}

static int
atomic_test_gt_int32(uint32_t p,uint32_t v)
{
    int ret;
    __asm__ volatile(
        "1:\tll %0, %3\n\t"
        "beq    %1, %2, 2f\n\t"
        "li     $0, 0\n\t"
        "sc     %0, %1\n\t"
        "bgt    %0, 1b\n\t"
        "j  3f\n\t"
        "2:\n\t"
        "li     %0, 1"
        "3:\n\t"
        :"=&r"(ret),"=m" (p)
        :"r" (v),"m" (p)
        :"memory"
    );

    return (ret);
}

static int
atomic_test_ge_int32(uint32_t p,uint32_t v)
{
    int ret;
    __asm__ volatile(
        "1:\tll %0, %3\n\t"
        "beq    %1, %2, 2f\n\t"
        "li     $0, 0\n\t"
        "sc     %0, %1\n\t"
        "bge    %0, 1b\n\t"
        "j  3f\n\t"
        "2:\n\t"
        "li     %0, 1"
        "3:\n\t"
        :"=&r"(ret),"=m" (p)
        :"r" (v),"m" (p)
        :"memory"
    );

    return (ret);
}

static int
atomic_test_lt_int32(uint32_t p,uint32_t v)
{
    int ret;
    __asm__ volatile(
        "1:\tll %0, %3\n\t"
        "beq    %1, %2, 2f\n\t"
        "li     $0, 0\n\t"
        "sc     %0, %1\n\t"
        "blt    %0, 1b\n\t"
        "j  3f\n\t"
        "2:\n\t"
        "li     %0, 1"
        "3:\n\t"
        :"=&r"(ret),"=m" (p)
        :"r" (v),"m" (p)
        :"memory"
    );

    return (ret);
}

static int
atomic_test_le_int32(uint32_t p,uint32_t v)
{
    int ret;
    __asm__ volatile(
        "1:\tll %0, %3\n\t"
        "beq    %1, %2, 2f\n\t"
        "li     $0, 0\n\t"
        "sc     %0, %1\n\t"
        "ble    %0, 1b\n\t"
        "j  3f\n\t"
        "2:\n\t"
        "li     %0, 1"
        "3:\n\t"
        :"=&r"(ret),"=m" (p)
        :"r" (v),"m" (p)
        :"memory"
    );

    return (ret);    
}

static inline void
atomic_set_int32(volatile uint64_t *p, uint64_t v)
{
    uint64_t tmp;
    __asm__ volatile(
        "1:\n\t"
        "lld    %0, %3\n\t"
        "or     %0, %2, %0\n\t"
        "scd    %0, %1\n\t"
        "beqz   %0, 1b\n\t"
        :"=&r" (tmp), "=m" (*p)
        :"r" (v), "m"(*p)
        :"memory"
    );
}

static inline void
atomic_clear_int64(volatile uint64_t *p, uint64_t v)
{
    uint64_t tmp;
    v = ~v;

    __asm__ volatile(
        "1:\n\t"
        "lld    %0, %3\n\t"
        "and    %0, %2, %3\n\t"
        "scd    %0, %1\n\t"
        "beqz   %0, 1b\n\t"
        : "=&r" (tmp) , "=m" (*p)
        : "r" (v), "m" (*p)
        : "memory"
    );
}

static inline void
atomic_add_int64(volatile uint64_t *p, uint64_t v)
{

}

static inline void
atomic_sub_int64(volatile uint64_t *p, uint64_t v)
{

}

static inline uint64_t 
atomic_test_eq_int64(uint64_t p, uint64_t v)
{

}

static inline uint64_t 
atomic_test_ge_int64(uint64_t p, uint64_t v)
{

}

static inline uint64_t 
atomic_test_gt_int64(uint64_t p, uint64_t v)
{

}

static inline uint64_t 
atomic_test_le_int64(uint64_t p, uint64_t v)
{

}

static inline uint64_t 
atomic_test_lt_int64(uint64_t p, uint64_t v)
{

}

#define atomic_set_int(p,v)                 \
        atomic_set_int32((p),(v))

#define atomic_clear_int(p,v)               \
        atomic_clear_int32((p),(v))

#define atomic_add_int(p,v)                 \
        atomic_add_int32((p),(v))           

#define atomic_sub_int(p,v)                 \
        atomic_sub_int32((p),(v))

#define atomic_test_eq_int(p,v)             \
        atomic_test_eq_int32((p),(v))

#define atomic_test_gt_int(p,v)             \
        atomic_test_gt_int32((p),(v))

#define atomic_test_ge_int(p,v)             \
        atomic_test_ge_int32((p),(v))

#define atomic_test_lt_int(p,v)             \
        atomic_test_lt_int32((p),(v))

#define atomic_test_le_int(p,v)             \
        atomic_test_le_int32((p),(v))

#endif