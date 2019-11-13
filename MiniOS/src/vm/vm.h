#ifndef _VM_VM_H_
#define _VM_VM_H_

struct mmspace {
    vmspace_t       mms_vmspace;
    segsz_t         mms_swrss;
    segsz_t         mms_tsize;
    segsz_t         mms_dsize;
    segsz_t         mms_ssize;
    caddr_t         mms_taddr;
    caddr_t         mms_daddr;
    caddr_t         mms_maxsaddr;
};

#endif