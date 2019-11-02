#ifndef _SYS_SIGNAL_H_
#define _SYS_SIGNAL_H_

#define SIGABRT         0x000000001                     /* abort                        */
#define SIGALRM         0x000000002                     /* alarm                        */
#define SIGBUS          0x000000004                     /* hardware error               */
#define SIGCANCEL       0x000000008                     /* for thread library           */
#define SIGCHLD         0x000000010                     /* child proc status changed    */
#define SIGCONT         0x000000020                     /* continue running             */
#define SIGEMT          0x000000040                     /* hardware error               */
#define SIGFPE          0x000000080                     /* arithmetic error             */
#define SIGFREEZE       0x000000100                     /* check point freeze           */
#define SIGHUP          0x000000200                     /* disconnect                   */
#define SIGILL          0x000000400                     /* illegal hardware instrutment */
#define SIGINFO         0x000000800                     /* keyboard status request      */
#define SIGINT          0x000001000                     /* terminal interupt symbol     */
#define SIGKILL         0x000002000                     /* exit                         */
#define SIGLOST         0x000004000                     /* resource lost                */
#define SIGLWP          0x000008000                     /* for thread library           */
#define SIGPIPE         0x000010000                     /* for pipe used                */
#define SIGPOLL         0x000020000                     /* for poll                     */
#define SIGPROF         0x000040000                     /* timeout for prof timer       */
#define SIGPWR          0x000080000                     /* power invaild or resteart    */
#define SIGQUIT         0x000100000                     /* terminal exit                */
#define SIGSEGV         0x000200000                     /* invaild memory reference     */
#define SIGSTKFLT       0x000400000                     /* processor error              */
#define SIGSTOP         0x000800000                     /* stop                         */
#define SIGSYS          0x001000000                     /* invaild system call          */
#define SIGTERM         0x002000000                     /* terminated                   */
#define SIGTHAW         0x004000000                     /* check point thaw             */
#define SIGTSTP         0x008000000                     /* terminal stop symbol         */
#define SIGTRAP         0x010000000                     /* hardwware error              */
#define SIGTTIN         0x020000000                     /* tty read controll            */
#define SIGTTOU         0x040000000                     /* tty write controll           */
#define SIGVTALRM       0x080000000                     /* for virtual timmer           */
#define SIGWINCH        0x100000000                     /* terminal window size changed */
#define SIGXCPU         0x200000000                     /* over limit of CPU            */
#define SIGXFSZ         0x400000000                     /* over limit of file size      */
#define SIGXRES         0x800000000                     /* over limit of resources      */

struct sigevent {
    int                 se_signal;
};

#endif