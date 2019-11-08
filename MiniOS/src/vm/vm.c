#include <vm/vm.h>
#include <sys/lock.h>

static vm_offset_t
vm_map_min(vm_map_t map)
{
    return (map->vmm_header.vme_end);
}

static vm_offset_t
vm_map_max(vm_map_t map)
{
    return (map->vmm_header.vme_start);
}

vm_addr_t
vm_map_insert(vm_map_t map, vm_object_t object, vm_offset_t offset, vm_addr_t start, vm_offset_t end)
{
    if(start < vm_map_min(map) || end > vm_map_max(map)
            || start >= end ){
        return (KMEM_INVAILD_ADDRESS);
    }

    if(vm_map_look_entry(map, , &map->vmm_header)){

    }
}

void
vm_map_delete(vm_map_t map, vm_addr_t start, vm_offset_t end)
{

}


vm_addr_t
vm_map_findspace(vm_map_t map, vm_addr_t start, size_t size)
{

}