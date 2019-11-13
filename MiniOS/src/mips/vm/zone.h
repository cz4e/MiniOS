#ifndef _MIPS_ZONE_H_
#define _MIPS_ZONE_H_

#define PAGE_SIZE               4096 /* KB */
#define MAX_ZONETYPES           4
#define MAX_VMSPACE_POOL        1024
static const char *zone_name[MAX_ZONETYPES] = {
    "KSEG2 ZONE",
    "KSEG1 ZONE",
    "KSEG0 ZONE",
    "KUSEG ZONE"
};


#endif