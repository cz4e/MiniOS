#ifndef _MIPS_PMAP_H_
#define _MIPS_PMAP_H_

#define PMAP_LOCK_INIT(pmap)                      \
    spin_lock_init(&(pmap)->pmap_lock,,"pmap lock")

#endif