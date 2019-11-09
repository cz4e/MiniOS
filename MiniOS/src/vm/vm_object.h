#ifndef _VM_VM_OBJECT_H_
#define _VM_VM_OBJECT_H_

#include <sys/queue.h>

struct vm_object {
    LIST_HEAD(vm_page)          vmo_plist;
    objtype_t                   vmo_type;
    volatile int                vmo_refcnt;
};

typedef struct vm_object *      vm_object_t;
#endif