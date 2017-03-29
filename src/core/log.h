

/**
 * log.h
 */

#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <pthread.h>


#define TSIZE_KB(Y)    	(Y * 1024)
#define TSIZE_MB(X)    	(TSIZE_KB(X) * 1024)

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

/**
 * LOG
 */
typedef struct {
	char *path;
	FILE *hdl;
	long turn_size;
    pthread_mutex_t mutex;
}LOG_FILE;


enum{
	LOG_OK,		
	FULL_LOG_HEAP,
    LOG_SLOT_IS_EMPTY
};

void init_logfile(int ns);
void close_logfile(void);

int assign_logfile(char *path, long turn_size);
int deassign_logfile(int id);

int append_logfile(int id, char * str );
int sappend_logfile(int id,const char *fmt, ...);

#endif
