#include <sys/types.h>
#include <sys/queue.h>
#include <vm/zone.h>
#include <vm/node.h>
#include <vm/vmspace.h>
#include <sys/lock.h>
#include <bzero.h>


static struct vmspace vmspace_pool[MAX_VMSPACE_POOL];
static struct mtx vmspace_lock;
extern struct lock_class lock_class_spin;
extern struct zone zones[MAX_ZONETYPES];

static vmspace_t find_a_vaild_vmspace(void);
static void free_a_vmspace(vmspace_t);
static vmspace_t _vmspace_alloc(zone_t,int, vm_size_t);
static vmspace_t _vmspace_alloc_wait(zone_t,int ,vm_size_t);
static vmspace_t vmspace_node_select_and_divide(zone_t,node_t,int,vm_size_t);
static void vmspace_divide(zone_t,vmspace_t, int);
static vmspace_t vmspace_combined(vmspace_t ,vmspace_t);
static void _vmspace_free(zone_t, vmspace_t );



void
vmspace_startup(void)
{
    bzero(vmspace_pool,sizeof(struct vmspace) * MAX_VMSPACE_POOL);
    spin_lock_init(&vmspace_lock.lock_object,&lock_class_spin,"VMSPACE LOCK");
}

vmspace_t
vmspace_alloc(int zone_type, vm_size_t size)
{
    vmspace_t res;
    int order;

    zone_t zone = &zones[zone_type];
    size = round_base2(size);
    order = get_order_base2(size);
    ZONE_LOCK(&zone->zone_lock);
    res = _vmspace_alloc(zone,order, size);
    ZONE_UNLOCK(&zone->zone_lock);
    return (res);
}

static vmspace_t
find_a_vaild_vmspace(void)
{
    vmspace_t res = NULL;
    static int  index = 0;

    for(;;) {
        if(index < MAX_VMSPACE_POOL) {
            if(vmspace_pool[index].vm_present_size == 0 
                        && vmspace_pool[index].vm_initial_size == 0) {
                res = &vmspace_pool[index];
                break;
            } else {
                index = 0;
            }
        }
    }

    return (res);
}

static void
free_a_vmspace(vmspace_t nouesd)
{
    bzero(nouesd,sizeof(struct vmspace));
}

static vmspace_t
_vmspace_alloc(zone_t zone, int order, vm_size_t size)
{
    node_t present_node;
    vmspace_t res;

    if(order >= MAX_ORDER || order < 0) {
        panic("_vmspace_alloc: the order must less than MAX_ORDER and great than zero");
    }

    present_node = zone->zone_nodes[order];

    while(order < MAX_ORDER && present_node->node_nr_nodes == 0) {
        present_node = zone->zone_nodes[order];
        ++order;
    }

    if(order == MAX_ORDER) {
        panic("_vmspace_alloc: kernel resources exhausted");
    }

    res = vmspace_node_select_and_divide(zone, present_node,order, size);

    return (res);
}

static vmspace_t 
_vmspace_alloc_wait(zone_t zone, int order, vm_size_t size)
{
    node_t present_node;
    vmspace_t res;
    int old_order = order;
    if(order >= MAX_ORDER || order < 0) {
        panic("_vmspace_alloc: the order must less than MAX_ORDER and great than zero");
    }

    present_node = zone->zone_nodes[order];

    while(1) {
        while(order < MAX_ORDER && present_node->node_nr_nodes == 0) {
            present_node = zone->zone_nodes[order];
                ++order;
        }

        if(order == MAX_ORDER) {
            ZONE_UNLOCK(&zone->zone_lock);
            // sleep a little while;
            order = old_order;
        } else {
            break;
        }
    }

    res = vmspace_node_select_and_divide(zone, present_node,order, size);

    return (res);
}



static vmspace_t
vmspace_node_select_and_divide(zone_t zone, node_t node,int  order, vm_size_t size)
{
    vmspace_t res;
    vmspace_t source_space;
    int total;
    int reserve_pages, page_counter = 0;
    vmspace_t new_node;

/*
    Start allocation
*/
    res = find_a_vaild_vmspace(); /* get a vaild structure from pool */
    source_space = LIST_FIRST(&node->node_vmspace);
    LIST_REMOVE_HEAD(&node->node_vmspace,source_space,vm_entry);
    node->node_nr_nodes--;

    total = (size / PAGE_SIZE + 1) ;    /* pages */
    res->vm_present_size = total * PAGE_SIZE;
    res->vm_start = source_space->vm_start;
    res->vm_owner = source_space->vm_owner;
    res->vm_used = 1;
    LIST_INSERT_BEFORE(source_space,res,vm_buddy);
    reserve_pages = (powerof2(order) - round_base2(total)) / PAGE_SIZE;

#ifdef MEM_GROUW_DOWN
    source_space->vm_start -= total * PAGE_SIZE;
#else
    source_space->vm_start += total * PAGE_SIZE;
#endif

/*
    Here, allocation is done!
*/
/*
    Insert reserve pages into anoter order list
*/
    if(is_round_base2(reserve_pages)) {
        reserve_pages = round_base2(reserve_pages);
        LIST_INSERT_HEAD(&zone->zone_nodes[reserve_pages]->node_vmspace,source_space,vm_entry);
        zone->zone_nodes[reserve_pages]->node_nr_nodes++;
        goto success;
    }

    vmspace_divide(zone,source_space,reserve_pages);
/*
    Insert finished!
*/
success:

    return (res);
}

static  void
vmspace_divide(zone_t zone, vmspace_t source_space, int reserve_pages)
{
    int page_counter = 0;
    vmspace_t new_node;

    while(reserve_pages) {
        reserve_pages--;
        page_counter++;
        if(is_round_base2(reserve_pages)) {
            new_node = find_a_vaild_vmspace();
#ifdef MEM_GROW_DOWN
            new_node->vm_start = source_space->vm_start - PAGE_SIZE * page_counter;
#else
            new_node->vm_start = source_space->vm_start + PAGE_SIZE * page_counter;
#endif
            new_node->vm_present_size = reserve_pages * PAGE_SIZE;
            new_node->vm_initial_size = source_space->vm_initial_size;
            new_node->vm_owner = source_space->vm_owner;
            source_space->vm_present_size -= reserve_pages * PAGE_SIZE;

            reserve_pages = round_base2(reserve_pages);
            zone->zone_nodes[reserve_pages]->node_nr_nodes++;

            LIST_INSERT_HEAD(&zone->zone_nodes[reserve_pages]->node_vmspace,new_node,vm_entry);
            /* connect buddies   */
            LIST_INSERT_AFTER(source_space,new_node,vm_buddy);
            reserve_pages = page_counter;
            page_counter = 0;
        } else if(reserve_pages == 0) { /* only one page reserve */
            zone->zone_nodes[0]->node_nr_nodes++;
            LIST_INSERT_HEAD(&zone->zone_nodes[0]->node_vmspace,source_space,vm_entry);
        }
    }

    return ;
}

static vmspace_t
vmspace_combined(vmspace_t vmspace,vmspace_t buddy)
{
    LIST_NEXT(vmspace,vm_buddy) = LIST_NEXT(buddy,vm_buddy);

    if(LIST_PREV(buddy,vm_buddy) && (*LIST_PREV(buddy,vm_buddy))) {
        LIST_PREV(&LIST_NEXT(buddy,vm_buddy),vm_buddy) = &vmspace;
    } 

    vmspace->vm_present_size += buddy->vm_present_size;
    free_a_noused_vmspace(buddy);
    return (vmspace);
}


void
vmspace_free(vmspace_t vmspace)
{

    vmspace->vm_refcount--;
    zone_t zone = vmspace->vm_zone;
    if(vmspace->vm_refcount == 0) {
        _vmspace_free(zone,vmspace);
    } else if(vmspace->vm_refcount < 0) {
        panic("vmspace_free: vmspace has been freed already");
    } 
    return ;
}

static void
_vmspace_free(zone_t zone, vmspace_t vmspace)
{
    uint64_t size = vmspace->vm_present_size;
    vmspace_t vm = vmspace;
    uint64_t pages;

    while(LIST_NEXT(vm,vm_buddy) && !LIST_NEXT(vm,vm_buddy)->vm_used) {
        size += LIST_NEXT(vm,vm_buddy)->vm_present_size;
        vm = vmspace_combined(vm,LIST_NEXT(vm,vm_buddy));
    }

    while(LIST_PREV(vm,vm_buddy) && *LIST_PREV(vm,vm_buddy) && !(*LIST_PREV(vm,vm_buddy))->vm_used) {
        size += (*LIST_PREV(vm,vm_buddy))->vm_present_size;
        vm = vmspace_combined(*LIST_PREV(vm,vm_buddy),vm);
    }

    if(is_round_base2(size)) {
        size = get_order_base2(size);
        LIST_INSERT_HEAD(&zone->zone_nodes[size]->node_vmspace,vm,vm_entry);
    } else {
        pages = size / PAGE_SIZE;
        vmspace_divide(zone,vm,pages);
    }
    return ;
}