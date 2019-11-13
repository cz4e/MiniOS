#ifndef _VM_VMSPACE_H_
#define _VM_VMSPACE_H_

struct vmspace {
    LIST_ENTRY(vmspace)         vm_entry;
    LIST_ENTRY(vmspace)         vm_buddy;
    node_t                      vm_owner;
    caddr_t                     vm_start;
    vm_size_t                   vm_present_size;
    vm_size_t                   vm_initial_size;
    uint64_t                    vm_refcount;
    char                        vm_used;                /* for find_a_vaild_vmspace */
};

typedef struct vmspace *       vmspace_t;
#endif