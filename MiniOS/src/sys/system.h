#ifndef _SYS_SYSTEM_H_
#define _SYS_SYSTEM_H_

#ifdef __MIPS__
#include <mips/include/cdefs.h>
#elif defined __AMD64__
#include <amd64/include/cdefs.h>
#elif defined __X86__
#include <x86/include/cdefs.h>
#endif

#include <sys/pcpu.h>
#include <sys/lock.h>

static void 
critical_enter(int cpuid)
{
    struct thread *td = PCPU_GET(cpuid)->pcpu_curthread;
    
    thread_lock(&td->td_mtx);
    td->td_critnest++;
    thread_unlock(&td->td_mtx);
    __compiler_membar();
}

static void
critical_exit(int cpuid)
{
    struct thread *td = PCPU_GET(cpuid)->pcpu_curthread;
    thread_lock(&td->td_mtx);
    td->td_critnest--;
    thread_unlock(&td->td_mtx);
}


#endif