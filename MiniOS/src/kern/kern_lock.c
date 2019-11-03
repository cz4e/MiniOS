#include <sys/lock.h>
#include <sys/sched.h>
#include <sys/pcpu.h>

#ifndef __SMP__
extern struct thread *curthread;
#else
extern struct thread *curthread[MAX_CPU];
#endif

struct lock_class lock_class_spin {
    .lc_name = "spin lock",
    .lc_flags = LC_SPINLOCK | LC_RECURSABLE,
    .lc_lock = spin_lock,
    .lc_unlock = spin_unlock
};

struct lock_class lock_class_sleep {
    .lc_name = "sleep lock",
    .lc_flags = LC_SLEEPLOCK | LC_RECURSABLE,
    .lc_lock = sleep_lock,
    .lc_unlock = sleep_unlock
};

struct lock_class lock_class_mtx {
    .lc_name = "mutual lock",
    .lc_flags = LC_MTXLOCK | LC_RECURSABLE
    .lc_lock = mtx_lock,
    .lc_unlock = mtx_unlock
};

void 
lock_init(struct lock_object *lo,struct lock_object *class,const char *name,int flags)
{
    
}

void
spin_lock(struct mtx *m)
{
    while(atomic_test_eq_int(m->lock_object.lo_data,LOCKING)) ;
    atomic_set_int(m->lock_object.lo_data,LOCKING);
}

void
spin_unlock(struct mtx *m)
{
    atomic_set_int(m->lock_object.lo_data,LOCK_FREE);
}

void 
sleep_lock(struct mtx *m) 
{
    while(atomic_test_eq_int(m->lock_object.lo_data,LOCKING)) 
        sched_sleep(curthread);

    atomic_set_int(m->lock_object.lo_data,LOCKING);
}

void
sleep_unlock(struct mtx *m)
{
    atomic_set_int(m->lock_object.lo_data,LOCK_FREE);
}

void
mtx_lock(struct mtx *m)
{
    atomic_sub_int(m->lock_object.lo_data);
    while(atomic_test_lt_int(m->lock_object.lo_data,0)) ;
}

void
mtx_unlock(struct mtx *m)
{
    atomic_add_int(m->lock_object.lo_data,1);
}
