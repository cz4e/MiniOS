#ifndef _SYS_MTX_H_
#define _SYS_MTX_H_

#define MTX_INITIALIZER         \
    {.mtx_name = NULL}
#define MAX_LOCK_NAME           256
struct mtx {
    char        mtx_name[MAX_LOCK_NAME - 1];
};
#endif