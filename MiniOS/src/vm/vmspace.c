#include <sys/types.h>
#include <sys/queue.h>
#include <vm/zone.h>
#include <vm/node.h>
#include <vm/vmspace.h>



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
vmspace_node_select_and_divide(zone_t zone, node_t node, vm_size_t size)
{
    vmspace_t res;
    vmspace_t source_space;

    LIST_NEXT(res,vm_entry) = LIST_PREV(res,vm_entry) = NULL;

    source_space = LIST_FIRST(&node->node_vmspace);
    LIST_REMOVE_HEAD(&node->node_vmspace,source_space,vm_entry);
    node->node_nr_nodes--;

    res->vm_start = source_space->vm_start;
    res->vm_owner = source_space->vm_owner;

    if(size > source_space->vm_present_size / 2) {
        res->vm_present_size = source_space->vm_present_size;
        res->vm_initial_size = source_space->vm_present_size;
    } else {
        while(1) {
            
        }
    }
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