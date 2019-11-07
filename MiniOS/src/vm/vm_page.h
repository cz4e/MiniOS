#ifndef _VM_VM_PAGE_H_
#define _VM_VM_PAGE_H_


#include <sys/queue.h>
#include <sys/types.h>

struct vm_page{
    LIST_ENTRY(vm_page)     vmp_list;
    uint16_t                vmp_flags;
    vm_addr_t               vmp_paddr;
    uint32_t                vmp_refcnt;
    uint8_t                 vmp_order;
    char                    vmp_vaild;
    char                    vmp_dirty;
};

typedef struct vm_page *vm_page_t;

#endif