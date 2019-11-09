#include <vm/vm_FSMM.h>
#include <sys/zone.h>
#include <sys/proc.h>

static zone_t   FSMML_uzone;
static zone_t   FSMN_uzone;
static zone_t   FSMML_kzone;
static zone_t   FSMN_kzone;

extern struct proc *curproc;

#define NODE_SIZE(start, end)       (sizeof(struct vm_FSMN) + start - end)


static index_t
addr2index(vm_addr_t start,vm_addr_t base, size_t item_size ,int direct)
{
    index_t index;

    if(direct == 0) /* up */{
        index = (start - base) % item_size;
    } else /* down */ {
        index = (base - start) % item_size;
    }

    return (index);
}

void
vm_startup(void)
{
    FSMML_zone = zone_create("FSMML ZONE",sizeof(struct vm_FSMML));
    FSMN_zone = zone_create("FSMN ZONE",sizeof(struct vm_FSMN));
}

static void
vm_head_init(vm_head_t head,vm_addr_t start, vm_offset_t end,int flags, int mode)
{
    index_t index;
    vm_node_t node;

    node = vm_node_alloc(mode);
    vm_node_init(node,start,end);
    FSMML_LOCK_INIT(&head->vm_lock);
    index = addr2index(head,FSMM_BEGIN,sizeof(struct vm_FSMML),VM_GROW_UP);
    LIST_FIRST(&head->vm_node) = node;
    head->vm_total_size = start - end;
    head->vm_owner = curproc;
    head->vm_index = index;
    head->vm_flags = flags;
    head->vm_swrss = 0;
    head->vm_tsize = 0;
    head->vm_dsize = 0;
    head->vm_ssize = 0;
    head->vm_taddr = 0;
    head->vm_maxsaddr = 0;
    head->vm_mode = mode;
    head->vm_using = 0;
}

vm_head_t
vm_head_alloc(int mode)
{
    vm_head_t head;

    if(mode == USER_ZONE)
        head = zone_alloc(FSMML_uzone,sizeof(struct vm_FSMNL),ZONE_WAITOK);
    else
        head = zone_alloc(FSMML_kzone,sizeof(struct vm_FSMML),ZONE_WAITOK);

    if(head == NULL) {
        panic("vm_head_alloc: kernel resources exhaused");
        return (NULL);
    }
    vm_head_init(head);

    return (head);
}


void
vm_head_link(vm_head_t head, vm_node_t node)
{

}

void
vm_head_unlink(vm_head_t head, vm_node_t node)
{

}

int
vm_head_destory(vm_head_t head)
{
    VM_HEAD_LOCK(&head->vm_lock);
    vm_node_destory(LIST_FIRST(&head->vm_node));
    VM_HEAD_UNLOCK(&head->vm_lock);
    zone_free(head,
                head->vm_mode == USER_ZONE ? FSMML_uzone : FSMML_kzone,
                sizeof(struct vm_FSMML)
            );

    return (0);
}

static void
vm_node_init(vm_node_t node, vm_addr_t start, vm_offset_t end)
{
    LIST_PREV(node,vmn_entry) = LIST_NEXT(node,vmn_entry) = NULL;
    node->vmn_start = start;
    node->vmn_end = end;
}

vm_node_t
vm_node_alloc(int mode)
{
    vm_node_t node;
    if(mode == USER_ZONE)
        node = zone_alloc(FSMN_uzone,NODE_SIZE(start,end),ZONE_WAITOK);
    else
        node = zone_alloc(FSMN_kzone,NODE_SIZE(start,end),ZONE_WAITOK);

    return (node);
}


int
vm_node_destory(vm_node_t node)
{
    vm_head_t head = node->vmn_head;
    zone_free(node,
            head->mode == USER_ZONE ? FSMN_uzone : FSMN_kzone,
            NODE_SIZE(node->vmn_start,node->vmn_end)
            );
}

