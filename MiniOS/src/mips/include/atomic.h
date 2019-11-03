#ifndef _MIPS_ATOMIC_H_
#define _MIPS_ATOMIC_H_


#define ATOMIC_FUNCTION(type)                       \
static inline type                                  \
atomic_test_eq_##type(type m, type n){              \
                                                    \
}                                                   \
static inline type                                  \
atomic_test_gt_##type(type m, type n){              \
                                                    \
}                                                   \
static inline type                                  \
atomic_test_ge_##type(type m, type n){              \
                                                    \
}                                                   \
static inline type                                  \
atomic_test_le_##type(type m, type n){              \
                                                    \
}                                                   \
static inline type                                  \
atomic_test_lt_##type(type m, type n){              \
                                                    \
}                                                   \
void                                                \
atomic_set_##type(type m, type n){                  \
                                                    \
}                                                   \
void                                                \
atomic_add_##type(type first, type second){         \
                                                    \
}                                                   \
void                                                \
atomic_sub_##type(type first, type second){         \
                                                    \
}                                                   \

ATOMIC_FUNCTION(char)
ATOMIC_FUNCTION(short)
ATOMIC_FUNCTION(int)
ATOMIC_FUNCTION(long)

#endif