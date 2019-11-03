#ifndef _SYS_LOCK_H_
#define _SYS_LOCK_H_

#include <sys/_lock.h>
#include <sys/cdefs.h>

void spin_lock(struct mtx *m);
void spin_unloc(struct mtx *m);
void sleep_lock(struct mtx *m);
void sleep_unlock(struct mtx *m);
void mtx_lock(struct mtx *m);
void mtx_unlock(struct mtx *m);

#endif