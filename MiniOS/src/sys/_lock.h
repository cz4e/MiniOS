#ifndef _SYS__LOCK_H_
#define _SYS__LOCK_H_

#include <sys/types.h>
#define LO_NEW                  0x00000001
#define LO_RECURSABLE           0x00000002
#define LO_DUPOK                0x00000004
#define LO_INITIALIZED          0x00000008
#define LO_CLASSSHIFT           24
#define LC_SPINLOCK             (0x00000001 << LO_CLASSSHIFT)
#define LC_SLEEPLOCK            (0x00000002 << LO_CLASSSHIFT)
#define LC_RECURSABLE           (0x00000004 << LO_CLASSSHIFT)

struct lock_object {
    const char      *lo_name;
    int             lo_flags;
    int             lo_data;
};

struct lock_class {
    const char *        lc_name;
    flag_t              lc_flag;
    void                (*lc_lock)(const struct lock_object *lock,int what);
    uintptr_t           (*lc_unlock)(const struct lock_object *lock,int how);
};
#endif