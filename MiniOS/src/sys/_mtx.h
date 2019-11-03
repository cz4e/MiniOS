#ifndef _SYS__MTX_H_
#define _SYS__MTX_H_

#include <sys/_lock.h>
#define MTX_NEW                 0x00000001
#define MTX_RECURSE             0x00000002
#define MTX_DUPOK               0x00000004
#define MTX_SPIN                0x00000008
#define MTX_SLEEP               0x00000010
#define MTX_UNOWNED             0x00000020

struct mtx {
    struct lock_object      lock_object;
    struct lock_class       lock_class;
};

#endif