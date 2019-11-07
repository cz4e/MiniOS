#ifndef _VM_VM_OBJECT_H_
#define _VM_VM_OBJECT_H_

#include <sys/types.h>
#include <sys/queue.h>
#include <vm/vm_page.h>

LIST_HEAD(pglist,vm_page);

struct vm_object {
    LIST_ENTRY(vm_object) vmo_object;
    vm_size_t             vmo_size;
    struct pglist         vmo_pglist;
    volatile int          vmo_busy;
};

typedef struct vm_object *vm_object_t;

#endif