#ifndef _VM_ZONE_H_
#define _VM_ZONE_H_

#define MAX_ORDER           11

#define ZONE_DMA            0
#define ZONE_DMA32          1
#define ZONE_HIGHMEM        2
#define ZONE_NORMAL         4
#define ZONE_MOVABLE        5
#define MAX_ZONE_NR         (ZONE_MOVABLE + 1)

#include <mips/vm/zone.h>

struct node;
struct zone{
    struct mtx      zone_lock;
    /* init in zone_int */
    struct node     *zone_nodes[MAX_ORDER];
    uint64_t        zone_free_area;
    int             zone_type;
    int             zone_flags;
    const char *    zone_name;
};

typedef struct zone *   zone_t;
#endif