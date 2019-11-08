#ifndef _VM_VM_H_ 
#define _VM_VM_H_
#include <vm/vm_object.h>
#include <mips/vm/vm.h>
#include <sys/lock.h>

struct vm_map;
struct vm_map_entry;
struct vm_object;
struct vmspace;

typedef struct vm_map *vm_map_t;
typedef struct vm_map_entry vm_map_entry_t;
typedef struct vmspace *vmspace_t;

struct vmspace {
    struct mtx          vms_lock;
    struct vm_map       vms_map;
    segsz_t             vms_swrss;
    segsz_t             vms_tsize;
    segsz_t             vms_dsize;
    segsz_t             vms_ssize;
    caddr_t             vms_taddr;
    caddr_t             vms_daddr;
    caddr_t             vms_maxsaddr;
    int                 vms_refcnt;
};

struct vm_map {
    struct vm_map_entry         vmm_header;
    struct mtx                  vmm_lock;
    vm_size_t                   vmm_size;
    uint32_t                    vmm_timestamp;
    vm_flags_t                  vmm_flags;
};

typedef union  {
    struct vm_object *vm_object;
    struct vm_map *submap;
} vm_map_object_t;

struct vm_map_entry {
    struct mtx          vme_lock;
    LIST_ENTRY(vm_map_entry)
                        vme_entries;
    vm_addr_t           vme_start;
    vm_offset_t         vme_end;
    vm_size_t           vme_maxfree;
    vm_flags_t          vme_flags;
    
    vm_map_object_t     vme_object;
};



vm_addr_t  vm_map_insert(vm_map_t ,vm_object_t,vm_offset_t, vm_addr_t, vm_offset_t);
void vm_map_delete(vm_map_t ,vm_addr_t, vm_offset_t );

#define KMEM_INVAILD_ADDRESS            -1

#endif