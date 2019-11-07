#include <vm/vm.h>

vm_offset_t
kmap_alloc(vm_map_t map, size_t size)
{
    vm_addr_t addr;
    size = size % PAGE_SIZE;

    for(;;) {
        vm_map_lock(&map->vmm_lock);
        addr = vm_map_findspace(map,vm_map_min(map), size);
        if(addr + size <= vm_map_max(map))  {
            break;
        } 
#ifdef SWAP
        else {
            vm_map_unlock(&map->vmm_lock);
            swap_release(size);
            return (0);
        }
#endif
    }   
    vm_map_insert(map, NULL, 0, addr, addr + size);
    vm_map_unlock(&map->vmm_lock); 

    return (addr);
}

void
kmap_free(vm_map_t map, vm_addr_t start, vm_offset_t offset)
{
    vm_map_lock(&map->vmm_lock);
    vm_map_delete(map, trunc_page(addr), (addr + size) % PAGE_SIZE);
    vm_map_unlock(&map->vmm_lock);
}