#ifndef _VM_VM_H_
#define _VM_VM_H_

#include <sys/queue.h>
#include <sys/types.h>
#include <sys/lock.h>

struct vm_object;
struct vmspace;
typedef struct vm_object *  vm_object_t;
typedef struct vmspace *    vmspace_t;

struct vmspace {
    struct mtx              vms_lock;
    LIST_ENTRY(vmspace)     vms_entry;
    struct vm_object        vms_first;
    segsz_t                 vms_swrss;
    segsz_t                 vms_tsize;
    segsz_t                 vms_dsize;
    segsz_t                 vms_ssize;
    segsz_t                 vms_taddr;
    caddr_t                 vms_daddr;
    caddr_t                 vms_maxaddr;
};

struct vm_object {
    struct mtx          vmo_lock;
    vm_addr_t           vmo_start;
    vm_offset_t         vmo_size;
    vm_object_t         vmo_next;
};


#endif