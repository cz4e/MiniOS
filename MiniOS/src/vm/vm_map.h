#ifndef _VM_VM_MAP_H_
#define _VM_VM_MAP_H_

#include <sys/queue.h>
#include <sys/types.h>
#include <sys/lock.h>
#include <vm/pmap.h>
#include <vm/vm_object.h>

struct vmspace;
struct vm_map;
struct vm_map_entry;

typedef struct vmspace *            vmspace_t;
typedef struct vm_map  *            vm_map_t;
typedef struct vm_map_entry *       vm_map_entry_t;


struct vmspace {
    struct mtx      vms_lock;
    struct vm_map   vms_map;
    segsz_t         vms_swrss;
    segsz_t         vms_tsize;
    segsz_t         vms_dsize;
    segsz_t         vms_ssize;
    caddr_t         vms_taddr;
    caddr_t         vms_daddr;
    caddr_t         vms_maxsaddr;
    volatile int    vms_refcnt;

    struct pmap     vms_pmap;
};


struct vm_map {
    struct mtx              vmm_lock;
 //   struct vm_map_entry     
    LIST_HEAD(,vm_map_entry)vmm_header;
    int                     vmm_nentries;
    vm_offset_t             vmm_size;
    char                    vmm_kernel_map;
    uint64_t                vmm_flags;
    pmap_t                  vmm_pmap;
    int                     vmm_busy;
};

struct vm_map_entry {
    LIST_ENTRY(vm_map_entry)    vme_entry;
    vm_addr_t                   vme_start;
    vm_offset_t                 vme_end;
    struct vm_object            vme_object;
    uint64_t                    vme_flags;
    vm_offset_t                 vme_next_read;
};

#endif