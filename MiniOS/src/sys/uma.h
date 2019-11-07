#ifndef _SYS_UMA_H_
#define _SYS_UMA_H_


void *uma_alloc(size_t , int flags);
void *uma_zalloc(size_t , int flags);

#endif