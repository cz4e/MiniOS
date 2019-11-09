#ifndef _VM_VM_PAGE_H_
#define _VM_VM_PAGE_H_

#include <sys/queue.h>

struct vm_page {
    LIST_ENTRY(vm_page)             vmp_entry;
    int                             vmp_flags;
    char                            vmp_vaild;
    char                            vmp_dirty;
    int                             vmp_refcnt;
};

typedef struct vm_page * vm_page_t;
#endif