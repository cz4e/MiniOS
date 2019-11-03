#ifndef _SYS__LOCK_H_
#define _SYS__LOCK_H_

#include <sys/types.h>

#define LO_SPINLOCK             0x00000001
#define LO_SLEEPLOCK            0x00000002
#define LO_MTXLOCK              0x00000004
#define LO_RECURSEABLE          0x00000008
#define LO_INITIALIZED          0x00000010
#define LO_DUPOK                0x00000020
#define LO_CLASSSHIFT           24

#define LC_SPINLOCK             0x00000001
#define LC_SLEEPLOCK            0x00000002
#define LC_MTXLOCK              0x00000004
#define LC_RECURSABLE           0x00000008
#define LC_DUPOK                0x00000010
#define LC_INITIALIZED          0x00000020
#define LC_MAX                  LC_INITIALIZED

#define LOCKING                 1
#define LOCK_FREE               0

struct lock_object {
    const char      *lo_name;
    int             lo_flags;
    int             lo_data;
};

struct lock_class {
    const char *        lc_name;
    int                 lc_flags;
    void                (*lc_lock)(const struct lock_object *lock,int what);
    uintptr_t           (*lc_unlock)(const struct lock_object *lock,int how);
};

struct mtx {
    struct lock_object      lock_object;
    struct lock_class       lock_class;
};

#endif