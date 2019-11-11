#include <sys/types.h>
#include <sys/queue.h>
#include <vm/zone.h>
#include <vm/node.h>
#include <vm/vmspace.h>

static struct vmspace node_pool[MAX_VMSPACE_POOL];

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

static vm_addr_t
vmspace_find_buddy(vmspace_t vmspace)
{
    vm_addr_t buddy_address;
    buddy_address = vmspace->vm_start + vmspace->vm_initial_size;

    return (buddy_address);
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

    res = vmspace_node_select_and_divide(zone, present_node, size);

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

    LIST_NEXT(res,vm_entry) = LIST_PREV(res,vm_entry) = NULL;

    source_space = LIST_FIRST(&node->node_vmspace);
    LIST_REMOVE_HEAD(&node->node_vmspace,source_space,vm_entry);
    node->node_nr_nodes--;

    res->vm_start = source_space->vm_start;
    res->vm_owner = source_space->vm_owner;

    total = (size / PAGE_SIZE + 1) ;
    reserve_pages = (powerof2(order) - round_base2(total)) / PAGE_SIZE;

#ifdef MEM_GROUW_DOWN
    source_space->vm_start -= total * PAGE_SIZE;
#else
    source_space->vm_start += total * PAGE_SIZE;
#endif

    if(is_round_base2(reserve_pages)) {
        reserve_pages = round_base2(reserve_pages);
        LIST_INSERT_HEAD(&zone->zone_nodes[reserve_pages]->node_vmspace,source_space,vm_entry);
        goto success;
    }

    while(reserve_pages) {
        reserve_pages--;
        page_counter++;
        if(is_round_base2(reserve_pages)) {
            new_node = find_a_vaild_node();
            new_node->vm_start = source_space->vm_start;
            new_node->vm_present_size = page_counter * PAGE_SIZE;
            new_node->vm_initial_size = source_space->vm_initial_size;
            new_node->vm_owner = source_space->vm_owner;
#ifdef MEM_GROUW_DOWN
            source_space->vm_start -= page_counter * PAGE_SIZE;
#else
            source_space->vm_start += page_counter * PAGE_SIZE;
#endif
            reserve_pages = round_base2(reserve_pages);
            page_counter = round_base2(page_counter);
            zone->zone_nodes[reserve_pages]->node_nr_nodes++;
            zone->zone_nodes[page_counter]->node_nr_nodes++;

            LIST_INSERT_HEAD(&zone->zone_nodes[reserve_pages]->node_vmspace,source_space,vm_entry);
            LIST_INSERT_HEAD(&zone->zone_nodes[page_counter]->node_vmspace,new_node,vm_entry);
            goto success;
        } else if(reserve_pages == 0) { /* only one page reserve */
            new_node = find_a_vaild_node();
            new_node->vm_start = source_space->vm_start;
            new_node->vm_present_size = PAGE_SIZE;
            new_node->vm_initial_size = source_space->vm_initial_size;
            new_node->vm_owner = source_space->vm_owner;

            zone->zone_nodes[0]->node_nr_nodes++;
            LIST_INSERT_HEAD(&zone->zone_nodes[0]->node_vmspace,new_node,vm_entry);
        }
    }
    
success:

    return (res);
}


vmspace_t 
_vmspace_alloc_wait(zone_t zone, int order, vm_size_t size)
{

}

void
vmspace_free(zone_t zone, vmspace_t vmspace)
{

}