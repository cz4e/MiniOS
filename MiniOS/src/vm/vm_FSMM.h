#ifndef _VM_VM_FSMM_H_
#define _VM_VM_FSMM_H_

#include <sys/types.h>
#include <sys/queue.h>
#include <sys/lock.h>

struct proc;
struct thread;

struct vm_FSMML {
    struct mtx                  vm_lock;
    LIST_HEAD(,vm_FSMN)         vm_node;
    vm_size_t                   vm_total_size;
    struct proc *               vm_owner;
    index_t                     vm_index;
    int                         vm_flags;
    segsz_t                     vm_swrss;
    segsz_t                     vm_tsize;
    segsz_t                     vm_dsize;
    segsz_t                     vm_ssize;
    caddr_t                     vm_taddr;
    caddr_t                     vm_daddr;
    caddr_t                     vm_maxsaddr;
    char                        vm_mode;
    char                        vm_using;
};

struct vm_FSMN {
    LIST_ENTRY(vm_FSMN)         vmn_entry;
    vm_head_t                   vmn_head;
    vm_addr_t                   vmn_start;
    vm_offset_t                 vmn_end;
};

typedef struct vm_FSMNL       *vm_head_t;
typedef struct vm_FSMN        *vm_node_t;

void vm_startup(void);

vm_head_t vm_head_alloc(int mode);
void vm_head_link(vm_head_t head, vm_node_t node);
void vm_head_unlink(vm_head_t head, vm_node_t node);
int vm_head_destory(vm_head_t head);
vm_node_t vm_node_alloc(int mode);
int vm_node_destory(vm_node_t node);
#endif