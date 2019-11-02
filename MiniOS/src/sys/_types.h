#ifndef _SYS__TYPES_H_
#define _SYS__TYPES_H_


#ifdef __MIPS__
#include <mips/include/_types.h>
#endif

typedef unsigned int        __useconds_t;
typedef __int64_t           __pid_t;
typedef __int32_t           __tid_t;
typedef __int32_t           __flag_t;
typedef __int32_t           __cpuid_t;
typedef __int32_t           __signal_t;
typedef __int32_t           __sigmask_t;

#if     defined __64__
typedef __int64_t           __vm_addr_t;
typedef __int64_t           __vm_offset_t;
#elif   defined __32__
typedef __int32_t           __vm_addr_t;
typedef __int32_t           __vm_offset_t;
#elif   defined __16__
typedef __int16_t           __vm_addr_t;
typedef __int16_t           __vm_offset_t;
#endif

typedef __vm_offset_t       __size_t;


#endif