#include <vm/vm_page.h>
#include <vm/vm_object.h>
#include <vm/vm_map.h>
#include <vm/uma.h>
#include <sys/lock.h>
#include <mips/vm/pmap.h>
#include <bzero.h>
#include <sys/types.h>
#include <mips/include/atomic.h>
#include <sys/queue.h>

extern struct thread    *curthread;
static uma_zone_t       mapentzone;
static uma_zone_t       kmapentzone;
static uma_zone_t       mapzone;
static uma_zone_t       vmspacezone;

static pmap_t 
vmspace_pmap(vmspace_t vms)
{
    return (vms->vms_pmap);
}

static void 
vmspae_zinit(void *addr, int size, int flags)
{
    struct vmspace *vms;

    vms = (struct vmspace *) addr;
    vm_map_zinit(&vms->vms_map,sizeof(vms->vms_map),flags);
    PMAP_LOCK_INIT(vmspace_pmap(vms));

}

static void
vm_map_zinit(void *addr, int size, int flags)
{
    vm_map_t    vmm;
    vmm = (vm_map_t)addr;
    bzero(vmm,sizeof(struct vm_map));
    vm_map_lock_init(&vmm->vmm_lock);
}

void
vm_map_startup(void)
{
    mapzone = uma_zcreate("map zone",sizeof(struct vm_map),vm_map_zinit,);
    kmapentzone = uma_zcreate("kernel map entry zone",sizeof(struct vm_map_entry),NULL,);
    mapentzone = uma_zcreate("map entry zone",sizeof(struct vm_map_entry),NULL,);
    vmspacezone = uma_zcreate("vmspace zone",sizeof(struct vmspace),vmspae_zinit,);
}


static vm_map_entry_t
vm_map_entry_create(vm_map_t vmm)
{
    vm_map_entry_t new_entry;

    if(vmm->vmm_kernel_map) {
        new_entry = uma_zalloc(kmapentzone);
    } else {
        new_entry = uma_zalloc(mapentzone);
    }

    if(new_entry == NULL) {
        panic("vm_map_entry_create: kernel resources exhausted");
    }
    return (new_entry);
}


static void
_vm_map_init(vm_map_t vmm, pmat_t pmap, vm_offset_t min, vm_offset_t max)
{
    LIST_FIRST(&vmm->vmm_header) = vm_map_entry_create(vmm);
    LIST_FIRST(&vmm->vmm_header)->vme_start = max;
    LIST_FIRST(&vmm->vmm_header)->vme_end = min;
    LIST_FIRST(&vmm->vmm_header)->vme_flags = MAP_ENTRY_HEADER;
    LIST_NEXT(LIST_FIRST(&vmm->vmm_header),vme_entry) = NULL;
    LIST_PREV(LIST_FIRST(&vmm->vmm_header),vme_entry) = NULL;
    vmm->vmm_kernel_map = 0;
    vmm->vmm_flags = 0;
    vmm->vmm_busy = 0;
    vmm->vmm_pmap = pmap;
}

vm_map_t
vm_map_create(pmap_t pmap,vm_offset_t min, vm_offset_t max)
{
    vm_map_t vmm;
    vmm = uma_zalloc(mapzone);
    _vm_map_init(vmm,pmap,min,max);
    return (vmm);
}

static void
vm_map_entry_delete(vm_map_t vmm,vm_map_entry_t entry)
{
    vm_map_entry_t cur;
    vm_object_t   cur_object;
    size_t   size;
    LIST_REMOVE(entry,vme_entry);
    cur_object = &entry->vme_object;

    size = entry->vme_end - entry->vme_start;
    vmm->vmm_size -= size;

    if(vmm->vmm_kernel_map) {
        vm_map_entry_deallocate(entry, KERNEL_MAP);
    } else {
        LIST_NEXT(entry,vme_entry) = curthread->td_map_def_user;
        curthread->td_map_def_user = entry;
    }
}

static void
vm_map_entry_deallocate(vm_map_entry_t vme, char mode) 
{
    vm_object_deallocate(vme->vme_object);
    uma_zfree(mode ? kmapentzone: mapentzone,entry);
}

vmspace_t
vmspace_alloc(vm_offset_t min, vm_offset_t max, pmap_init_t pinit)
{
    vmspace_t   vms;

    vms = uma_alloc(vmspacezone);

    if(!pinit(vmspace_pmap(vms))) {
        uma_zfree(vmspacezone,vms);
        return (NULL);
    }

    _vm_map_init(&vms->vms_map,vmspace_pmap(vms),min,max);
    
    vms->vms_refcnt = 1;
    vms->vms_swrss = 0;
    vms->vms_tsize = 0;
    vms->vms_dsize = 0;
    vms->vms_ssize = 0;
    vms->vms_taddr = 0;
    vms->vms_daddr = 0;
    vms->vms_maxsaddr = 0;

    return (vms);
}

static inline void
vmspace_dofree(vmspace_t vms)
{
    vm_map_remove(&vms->vms_map,vm_map_min(&vms->vms_map),vm_map_max(&vms->vms_map));
    pmap_release(vmspace_pmap(vms));
    vms->vms_map.vmm_pmap = NULL;
    uma_zfree(vmspacezone,vms);
}

void
vmspace_free(vmspace_t vms)
{
    if(vms->vms_refcnt == 0)
        return ;
    if(atomic_fetchadd_int(&vms->vms_refcnt,-1) == 1)
        vmspace_dofree(vms);

    return ;
}

int
vm_map_remove(vm_map_t vmm, vm_offset_t start, vm_offset_t end)
{
    int result;
    vm_map_lock(&vmm->vmm_lock);
    VM_MAP_RANGE_CHECK(vmm,start,end);
    result = vm_map_delete(vmm,start,end);
    vm_map_unlock(&vmm->vmm_lock);
}

int
vm_map_delete(vm_map_t vmm,vm_offset_t start, vm_offset_t end)
{

}

int
vm_map_lookup_entry(vm_map_t vmm, vm_offset_t addr, vm_map_entry_t vme)
{
    vm_map_entry_t      cur;

    if(LIST_FIRST(&vmm->vmm_header) == NULL) {
        vme == NULL;
        return (0);
    } else {
        LIST_SEARCH(cur,&vmm->vmm_header,vme_entry,cur->vme_start <= addr && cur->vme_end > addr);
    }

    if(cur == NULL)
        return (0);
    else {
        vme = cur;
        return (1);
    }

    vme = NULL;
    return (0);
}

static int
vm_map_stack_locked(vm_map_t vmm,vm_offset_t addr, vm_size_t max_ssize,vm_size_t growsize, int mode)
{
    vm_map_entry_t new_entry,prev_entry;
    vm_offset_t bot,top, gap_bot, gap_top;  
    vm_size_t init_ssize, sgp;
    int ret;

    init_ssize = growsize;
    if(vm_map_lookup_entry(vmm,addr,&prev_entry)) {
        return (KMEM_NO_MORE_SPACE);
    }

    if(LIST_NEXT(prev_entry,vme_entry)->vme_start < addr + max_ssize)
        return (KMEM_NO_MORE_SPACE);
    
    if(mode & MAP_STACK_GROWS_DOWN) {
        bot = addr + max_ssize - init_ssize;
        top = bot + init_ssize;
        gap_bot = addr;
        gap_top = bot;
    } else if( mode & MAP_STACK_GROWS_UP) {
        bot = addr;
        top = bot + init_ssize;
        gap_bot = top;
        gap_top = addr + max_ssize;
    }

    ret = vm_map_insert(vmm,NULL,0,bot,top,mode);
    if(ret != KMEM_SUCCESS) 
        return (ret);
    new_entry = LIST_NEXT(prev_entry,vme_entry);
//    prev_entry->vme_entry.le_next;
    ret = vm_map_insert(vmm,NULL,0,gap_bot,gap_top,mode);
    
    if(gap_bot == gap_top)
        return (KMEM_SUCCESS);
    
    if(ret == KMEM_SUCCESS) {
/*        if(mode & MAP_STACK_GROWS_DOWN) {
            LIST_PREV(new_entry,vme_entry)->vme_next_read = sgp;
        } else {
            LIST_NEXT(new_entry,vme_next)->vme_next_read = sgp;
        }
*/
        ;
    } else {
        vm_map_delete(vmm,bot,top);
    }

    return (ret);
}

int
vm_map_stack(vm_map_t vmm, vm_offset_t addr, vm_size_t max_ssize, int mode)
{
    vm_size_t growsize, init_ssize;
    rlim_t  vmemlim;
    int ret = KMEM_SUCCESS;

    growsize = 1;
    init_ssize = (max_ssize < growsize) ? max_ssize : growsize;

    vm_map_lock(&vmm->vmm_lock);
    vmemlim = lim_cur(curthread,RLIMIT_VMEM);

    if(vmm->vmm_size + init_ssize > vmemlim) {
        vm_map_unlock(&vmm->vmm_lock);
        return (KMEM_NO_MORE_SPACE);
    }

    ret = vm_map_stack_locked(vmm,addr,max_ssize,growsize, mode);
    vm_map_unlock(&vmm->vmm_lock);

    return (ret);
}





