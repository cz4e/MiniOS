#ifndef _SYS_MTX_H_
#define _SYS_MTX_H_

#include <sys/_mtx.h>
#define MTX_INITIALIZER         \
    {.mtx_name = NULL}
#define MAX_LOCK_NAME           256
#define MTX_READ_VALUE(m)       ((m)->mtx_lock)
#define _mtx_owner(v)           ((struct thread *)((v) & ~MTX_FLAGMASK))
#define mtx_owner(m)            _mtx_owner(MTX_READ_VALUE(m))
#define mtx_owned(m)            (mtx_owner(m) == curthread)
#define mtx_name(m)             ((m)->mtx_lockobj.lo_name)

#define mtx_init(c,name,opts)   _mtx_init((c),(name),(opts))
#define mtx_destory(c)          _mtx_destory((c))
#define mtx_lock_flags_(m,opts,file,line)               \
        _mtx_lock_flags((m),(opts),(file),(line))
#define mtx_unlock_flags_(m,opts,file,line)             \
        _mtx_unlock_flags((m),(opts),(file),(line))
#define mtx_lock_flags_(m,opts)                         \
        mtx_lock_flags_((m),(opts),LOCK_FILE,_LOCK_LINE)
#define mtx_unlock_flags(m,opts)                        \
        mtx_unlock_flags_((m),(opts),LOCK_FILE,LOCK_LINE)
#define mtx_lock(m)              mtx_lock_flags((m),0)
#define mtx_unlock(m)            mtx_unlock_flags((m),0)


#endif