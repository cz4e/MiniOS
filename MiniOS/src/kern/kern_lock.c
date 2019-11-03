#include <sys/lock.h>


static void 
lock_init(struct lock_object *lo,int class,const char *name,int flags)
{
    
    switch(class) {
        case LC_SLEEPLOCK:
            lo->lo_flags |= LC_SLEEPLOCK;
            break;
        case LC_SPINLOCK:
            lo->lo_flags |= LC_SPINLOCK;
            break;
    }

    lo->lo_name = name;
    lo->lo_flags = flags | LO_INITIALIZED;
}

