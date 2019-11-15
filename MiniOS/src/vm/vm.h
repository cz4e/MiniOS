#ifndef _VM_VM_H_
#define _VM_VM_H_

struct mmspace {
    caddr_t         mms_start;
    segsz_t         mms_swrss;
    segsz_t         mms_tsize;
    segsz_t         mms_dsize;
    segsz_t         mms_ssize;
    caddr_t         mms_taddr;
    caddr_t         mms_daddr;
    caddr_t         mms_maxsaddr;
    vm_size_t       mms_size;
};

#endif