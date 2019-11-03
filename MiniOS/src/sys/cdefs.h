#ifndef _SYS_CDEFS_H_
#define _SYS_CDEFS_H_
#include <sys/_types.h>

#ifndef __DEQUALIFY
#define __DEQUALIFY(type,var)                                               \
    ((type)(__uintptr_t)(const volatile void *)(var))
#endif

#define __offsetof(type,field)                                              \
    ((__size_t)(__uintptr_t)((const volatile void *)&((type *)0)->field))

#define __containerof(x,s,m)                                                \
    __DEQUALIFY(s *,(const volatile char *)x - __offsetof(s,m));
//    const volatile __typeof(((s *)0)->m) *__x = (x);                        

#endif