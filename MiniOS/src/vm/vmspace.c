#include <sys/types.h>
#include <sys/queue.h>
#include <vm/zone.h>
#include <vm/node.h>
#include <vm/vmspace.h>
#include <sys/lock.h>

static struct vmspace vmspace_pool[MAX_VMSPACE_POOL];
static struct mtx vmspace_lock;

static int 
is_round_base2(uint64_t n)
{
    uint64_t m = 1, p = 0;

    if(n <= 0 )
        return (0);
    else {
        while(m < n) {
            m *= 2;
            p++;
        }
        if(m == n) {
            return (n);
        } else {
            return (0);
        }
    }
}

static uint64_t 
powerof2(uint64_t n)
{
    uint64_t f = 1;
    while(n--) f *= 2;

    return (f);
}

static uint64_t 
round_base2(uint64_t n)
{
    uint64_t f = 1 ,counter = 1;
    while(f < n) f = powerof2(counter++);
    return (f == 1 ? 2 : f);
}



vmspace_t
vmspace_alloc(zone_t zone, vm_size_t size)
{
    vmspace_t res;
    int order;

    order = round_base2(size);
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
    LIST_NEXT(res,vm_entry) = LIST_PREV(res,vm_entry) = NULL;

    source_space = LIST_FIRST(&node->node_vmspace);
    LIST_REMOVE_HEAD(&node->node_vmspace,source_space,vm_entry);
    node->node_nr_nodes--;

    total = (size / PAGE_SIZE + 1) ;    /* pages */
    res->vm_present_size = total * PAGE_SIZE;
    res->vm_start = source_space->vm_start;
    res->vm_owner = source_space->vm_owner;

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
        goto success;
    }

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
            reserve_pages = page_counter;
            page_counter = 0;
        } else if(reserve_pages == 0) { /* only one page reserve */
            new_node = find_a_vaild_vmspace();
            new_node->vm_start = source_space->vm_start;
            new_node->vm_present_size = PAGE_SIZE;
            new_node->vm_initial_size = source_space->vm_initial_size;
            new_node->vm_owner = source_space->vm_owner;

            zone->zone_nodes[0]->node_nr_nodes++;
            LIST_INSERT_HEAD(&zone->zone_nodes[0]->node_vmspace,new_node,vm_entry);
        }
    }
/*
    Insert finished!
*/
success:

    return (res);
}


vmspace_t 
_vmspace_alloc_wait(zone_t zone, int order, vm_size_t size)
{
    node_t present_node;
    vmspace_t res;

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
        } else {
            break;
        }
    }

    res = vmspace_node_select_and_divide(zone, present_node,order, size);

    return (res);
}

void
vmspace_free(zone_t zone, vmspace_t vmspace)
{
    uint64_t size = vmspace->vm_present_size;
}