#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_

#include <sys/_types.h>

typedef __uint8_t               uint8_t;
typedef __uint16_t              uint16_t;
typedef __uint32_t              uint32_t;
typedef __uint64_t              uint64_t;
typedef char *                  caddr_t;
typedef const char *            c_caddr_t;

typedef __register_t            register_t;
typedef __pid_t                 pid_t;
typedef __tid_t                 tid_t;
typedef __flag_t                flag_t;
typedef __cpuid_t               cpuid_t;
typedef __signal_t              signal_t;
typedef __sigmask_t             sigmask_t;


typedef __vm_addr_t             vm_addr_t;
typedef __vm_offset_t           vm_offset_t;
typedef __size_t                size_t;

typedef __time_t                time_t;
typedef __utime_t               utime_t;
#endif