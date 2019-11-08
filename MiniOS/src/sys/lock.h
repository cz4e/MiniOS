#ifndef _SYS_LOCK_H_
#define _SYS_LOCK_H_

#include <sys/_lock.h>
#include <sys/cdefs.h>

#define spin_lock_init(lo, lc, n)                       \
    _lock_init((lo),(lc),(n),LOCK_FREE)

#define sleep_lock_init(lo, lc, n)                      \
    _lock_init((lo),(lc),(n),LOCK_FREE)

#define mtx_lock_init(lo, lc, n, d)                     \
    _lock_init((lo),(lc),(n),(d))


void _lock_init(struct lock_object *lo,
                    struct lock_class *class, 
                            const char *name, int data);
void spin_lock(struct mtx *m);
void spin_unlock(struct mtx *m);
void sleep_lock(struct mtx *m);
void sleep_unlock(struct mtx *m);
void mtx_lock(struct mtx *m);
void mtx_unlock(struct mtx *m);

#define thread_lock(m)                                  \
    spin_lock((m))
#define thread_unlock(m)                                \
    spin_unlock((m))

#define map_lock(m)                                     \
    spin_lock((m))
#define map_unlock(m)                                   \
    spin_unlock((m))

#define vm_map_lock(m)                                  \
    spin_lock((m))
#define vm_map_unlock(m)                                \
    spin_unlock((m))

#endif