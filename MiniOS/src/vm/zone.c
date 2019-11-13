#include <sys/types.h>
#include <sys/queue.h>
#include <sys/lock.h>
#include <vm/vmspace.h>
#include <vm/node.h>
#include <vm/zone.h>


static struct zone zones[MAX_ZONETYPES];
extern struct lock_class lock_class_spin;

void
zone_startup(void)
{
    for(int zone = 0; zone < MAX_ZONETYPES;++zone) {
        spin_lock_init(&zones[zone].zone_lock.lock_object,&lock_class_spin,"ZONE LOCK");
        zone_init(&zones[zone],zones[zone].zone_type);
    }

    return ;
}

void 
zone_init(zone_t zone, int zone_type)
{
    if(zone_type >= 0 && zone_type < MAX_ZONETYPES) {
        zone->zone_name = zone_name[zone_type];
    } else {
        panic("zone_init: invaild zone type");
    }
}


