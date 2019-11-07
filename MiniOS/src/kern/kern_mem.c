#include <vm/vm.h>

typedef struct vm_map *vm_map_t;

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
    vm_map_insert();
    vm_map_unlock(&map->vmm_lock); 
}