#ifndef __ERROR_H__
#define __ERROR_H__

#include "macro_utils.h"
#include <assert.h>
#define MAX_MSG_SIZE 1024

#define _ERROR_FATAL(x,...) error_fatal(_EVAL(__LOGFILE), __FUNCTION__,__LINE__,x, ##__VA_ARGS__)
void error_fatal(const char *logfile, const char *Nfuncion, const int Nline, const char *fmt, ...);

#define _REGISTRAR(x,...) registrar(_EVAL(__LOGFILE),"ERROR", __FUNCTION__,__LINE__,x, ##__VA_ARGS__)
void registrar(const char *logfile, const char *header, const char *Nfuncion, const int Nline, const char *fmt, ...);

#define _LOG(h,x,...) registrar(_EVAL(__LOGFILE),"LOG"h, __FUNCTION__,__LINE__,x, ##__VA_ARGS__)
void registrar(const char *logfile, const char *header, const char *Nfuncion, const int Nline, const char *fmt, ...);
#define _NOLOG(h,x,...) 


#endif
