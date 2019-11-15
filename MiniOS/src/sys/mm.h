#ifndef _SYS_ZONE_H_
#define _SYS_ZONE_H_


#include <vm/zone.h>
#include <vm/node.h>
#include <vm/vmspace.h>

struct vmspaceq {
    vmspace_t           vmq_next;
    caddr_t             vmq_addr;  
};

void *kmalloc(vm_size_t , vm_size_t , int);
void kmfree(void *);

vmspace_t kmalloc_vmspace(int, vm_size_t, int);
void kmfree_vmspace(vmspace_t);
#endif