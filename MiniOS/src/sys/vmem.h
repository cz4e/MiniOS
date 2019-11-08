#ifndef _SYS_VMEM_H_
#define _SYS_VMEM_H_

#include <vm/vm.h>
#include <sys/types.h>

struct vmem_btag {
    LIST_ENTRY(vmem_btag)       vbt_seglist;
    vm_addr_t                   vbt_start;
    vm_offset_t                 vbt_size;
};

LIST_HEAD(vmem_freelist,vmem_btag);
LIST_HEAD(vmem_hashlist,vmem_btag);
LIST_HEAD(vmem_seglist,vmem_btag);


#define VMEM_HASHSIZE_MIN           16
#define VMEM_HASHSIZE_MAX           131072
#ifdef  defined _64
#define VMEM_MAXORDER               64
#elif  defined _32
#define VMEM_MAXORDER               32
#elif defined _16
#define VMEM_MAXORDER               16
#endif

struct vmem {
    struct mtx           vmem_lock;
    LIST_ENTRY(vmem)     vmem_alllist;
    struct vmem_freelist vmem_freelist[VMEM_MAXORDER];
    struct vmem_hashlist vmem_hashlist[VMEM_HASHSIZE_MIN];
    struct vmem_seglist  vmem_seglist;
};


#endif