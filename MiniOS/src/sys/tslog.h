#ifndef _SYS_TSLOG_H_
#define _SYS_TSLOG_H_

#ifdef TSLOG
void tslog(void *, int, const char *, const char *);
#define TSRAW(td,type,f,s)                  tslog((td),(type),(f),(s))
#else
#define TSRAW(td,type,f,s)
#endif

#define TS_ENTER        0
#define TS_EXIT         1
#define TS_THREAD       2
#define TS_EVENT        3

#define TSENTER()       TSRAW(curthread,TS_ENTER,__func__, NULL)
#define TSEXIT()        TSRAW(curthread,TS_EXIT,__func__,NULL)
#endif