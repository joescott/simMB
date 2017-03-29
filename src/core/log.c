

/**
 * log.c
 */


#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#define STRTIME_SIZE 	  128
#define STRTIME_FORMAT 	"%y%m%d_%H%M%S"
#define HEAD_FORMAT 	  "[%s] "
#define END_FORMAT 	"\r\n"


static LOG_FILE **log_file;
static int num_slots = 0;

void  
init_logfile(int ns)
{
	num_slots = ns;
	log_file = calloc(num_slots,sizeof(LOG_FILE *));
}

void  
close_logfile(void)
{
	while(num_slots > 0)
		deassign_logfile(--num_slots);
	free(log_file);
}


int 
assign_logfile(char *path, long turn_size)
{
	LOG_FILE **pl;   
	for(pl = log_file;
			pl-log_file < num_slots && *pl != NULL ;pl++);

	if(pl-log_file == num_slots)
		return -(FULL_LOG_HEAP);

	*pl = (LOG_FILE *)malloc(sizeof(LOG_FILE));	
	(*pl)->hdl = NULL;
	(*pl)->path = strdup(path);
	(*pl)->turn_size = turn_size;
    pthread_mutex_init(&(*pl)->mutex,NULL);
	return pl-log_file;
}

int 
deassign_logfile(int id)
{
	LOG_FILE *pl;   
    if((pl = log_file[id]) != NULL) {
		fclose(pl->hdl);
		free(pl->path);
        pthread_mutex_destroy(&pl->mutex);
		free(pl);
		log_file[id] = NULL;
		return LOG_OK; 
	}
    return -(LOG_SLOT_IS_EMPTY);
}

static void
get_timestamp(char *strtime, int size)
{
	struct timeval tv;
	struct tm* ptm;
	gettimeofday( &tv, NULL );
	ptm = gmtime( &tv.tv_sec);
	strftime(
			strtime, 
			size,
			STRTIME_FORMAT, ptm 
		);
    char *st =strdup(strtime);
    snprintf(strtime,size,"%s.%03d",st,(int)tv.tv_usec/1000);
    free(st);

}

static void
turn_test(LOG_FILE *pl, const char *strtime)
{
	char *auxbuff;
	struct stat buf;
	stat(pl->path, &buf);
    if(buf.st_size >= pl->turn_size ) {
		fclose( pl->hdl );
		pl->hdl = NULL;
		auxbuff = malloc(strlen(strtime)+strlen(pl->path)+3);
		sprintf(auxbuff,"%s_%s",pl->path,strtime);
		rename(pl->path,auxbuff);
		free(auxbuff);
	}

}

int 
append_logfile(int id, char *str )
{
	char strtime[STRTIME_SIZE];

	if(log_file[id]->hdl == NULL)
		log_file[id]->hdl = fopen( log_file[id]->path , "a" );
	get_timestamp(strtime,STRTIME_SIZE);

    pthread_mutex_lock(&(log_file[id]->mutex));
	fprintf( log_file[id]->hdl , 
             HEAD_FORMAT"%s"END_FORMAT, strtime, str);
	fflush( log_file[id]->hdl );
    pthread_mutex_unlock(&(log_file[id]->mutex));

	turn_test(log_file[id],strtime);

	return LOG_OK;
}

int 
sappend_logfile(int id,const char *fmt, ...)
{
	char strtime[STRTIME_SIZE];
	va_list ap;

	if(log_file[id]->hdl == NULL)
		log_file[id]->hdl = fopen( log_file[id]->path , "a" );
	get_timestamp(strtime,STRTIME_SIZE);

    pthread_mutex_lock(&(log_file[id]->mutex));
	fprintf( log_file[id]->hdl , 
             HEAD_FORMAT, strtime);
	va_start(ap, fmt);
	vfprintf(log_file[id]->hdl,fmt, ap);
	va_end(ap);
	fprintf( log_file[id]->hdl, END_FORMAT);
	fflush( log_file[id]->hdl );
    pthread_mutex_unlock(&(log_file[id]->mutex));

	turn_test(log_file[id], strtime);

	return LOG_OK;
}

