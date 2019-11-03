#include <sys/mtx.h>
#include <sys/cdefs.h>
#include <sys/lock.h>

#define mtxlock2mtx(c)                      \
    (__containerof(c,struct mtx,mtx_lock))

static void lock_spin(struct lock_object *lock,int how);
static void lock_mtx(struct lock_object *lock,int how);

static uintptr_t unlock_spin(struct lock_object *lock);
static uintptr_t unlock_mtx(struct lock_object *lock);

struct lock_class lock_class_mtx_spin {
    .lc_name = "spin mutex",
    .lc_flags = LC_SPINLOCK | LC_RECURSABLE,
    .lc_lock = lock_spin,
    .lc_unlock = unlock_spin
};

struct lock_class lock_class_mtx_sleep {
    .lc_name = "sleep mutex",
    .lc_flags = LC_SLEEPLOCK | LC_RECURSABLE,
    .lc_lock = lock_mtx,
    .lc_unlock = unlock_mtx
};

void 
_mtx_init(volatile uintptr_t *c, const char *name,  int opts)
{
    int flags = 0;
    struct mtx *m;
    int class;

    if(opts & MTX_SPIN)
        class = LC_SPINLOCK;
    else 
        class = LC_SLEEPLOCK;

    if(opts & MTX_NEW) 
        flags |= LO_NEW;
    if(opts & MTX_RECURSE)
        flags |= LO_RECURSABLE;
    if(opts & MTX_DUPOK)
        flags |= LO_DUPOK;

    lock_init(&m->mtx_lockobj,class,name,flags);
    m->mtx_lock = MTX_UNOWNED;
}

void
_mtx_destory(volatile uintptr_t *c)
{
    struct mtx *m;

    if(!mtx_owned(m)) {
        return (-1);
    } else {
        if(m->mtx_lockobj.lo_flags & LC_SPINLOCK) {
            spinlock_exit();
        } else {
            ;
        }
    }
    m = mtxlock2mtx(c);
    m->mtx_lock = MTX_DESTORY;
    lock_destory(&m->mtx_lockobj);
}


void
_mtx_lock_flags(volatile uintptr_t *c,int opts,const char *file,int line)
{
    //
}

void
_mtx_unlock_flags(volatile uintptr_t *c,int opts,const char *file,int line)
{
    
}





