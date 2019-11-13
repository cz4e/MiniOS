#include <sys/types.h>
#include <sys/queue.h>
#include <mm.h>
#include <sys/lock.h>

#define VMQ_LOCK(lock)                              \
    spin_lock((lock))

#define VMQ_UNLOCK(lock)                            \
    spin_unlock((lock))



static LIST_HEAD(,vmspace)  vmq;
static struct mtx vmq_lock;

void *
kmalloc(int zone,vm_size_t size, int flags)
{
    vmspace_t newspace;

    newspace = vmspace_alloc(zone,size);
    VMQ_LOCK(&vmq_lock);
    LIST_INSERT_HEAD(&vmq,newspace,vm_vmq);
    VMQ_UNLOCK(&vmq_lock);
    return ((void *) newspace->vm_start);
}

/*
    If you want to use this function,  get vmq lock first
*/
static vmspace_t
addr2vmspace(void *where)
{
    caddr_t addr = (caddr_t) where;

    vmspace_t vm;

    for( vm = LIST_FIRST(&vmq);vm;vm = LIST_NEXT(vm,vm_vmq)) {
        if(vm->vm_start == addr) {
            break;
        }
    }

    return (vm);
}


void
kmfree(void * where)
{
    vmspace_t vm;

    VMQ_LOCK(&vmq_lock);
    vm = addr2vmspace(where);

    if(vm == NULL) {
        VMQ_UNLOCK(&vmq_lock);
        panic("kmfree: invaild address");
    }
    if(LIST_PREV(vm,vm_vmq) == NULL) {
        LIST_REMOVE_HEAD(&vmq,vm,vm_vmq);
    } else {
        LIST_REMOVE(vm,vm_vmq);
    }
    VMQ_UNLOCK(&vmq_lock);
    vmspace_free(vm);

    return ;    
}