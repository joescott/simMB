
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <mysql/mysqld_error.h>
#include <math.h>
#include <stdarg.h>
#include <sys/file.h>

#include "error.h"
#include "utils.h"

void error_fatal(const char *logfile, const char *Nfuncion, const int Nline, 
        const char *fmt, ...)
{
  va_list ap;
  FILE* f;
  char *msg = (char*) malloc(sizeof(char)*MAX_MSG_SIZE);
  va_start(ap, fmt);
  vsnprintf(msg, MAX_MSG_SIZE, fmt, ap);
  va_end(ap);
  if((f=fopen(logfile,"a+")) != NULL)
  {
    flock(fileno(f),LOCK_EX);
      fprintf(f, "%s - Salida exigida por ERROR: en funcion: %s[%d] = %s\n",
              get_fulltime(), Nfuncion, Nline, msg);
    flock(fileno(f),LOCK_UN);
    fclose(f);
  }
  printf(" %s || %s[%d] \r\n",msg, Nfuncion,Nline);
  free(msg);
  sleep(1);

  exit(EXIT_FAILURE);
}

void registrar(const char *logfile, const char *header,
        const char *Nfuncion, const int Nline, const char *fmt, ...)
{
  va_list ap;
  FILE *f;
  char *msg = (char*) malloc(sizeof(char)*MAX_MSG_SIZE);
  va_start(ap, fmt);
  vsnprintf(msg, MAX_MSG_SIZE, fmt, ap);
  va_end(ap);
  if((f = fopen(logfile,"a+")) != NULL)
  {
    flock(fileno(f),LOCK_EX);
    fprintf(f, "%s - %s: en funcion: %s[%d] = %s\n",get_fulltime(),
            header,Nfuncion,Nline,msg);
    flock(fileno(f),LOCK_UN);
    fclose(f);
  }
  free(msg);
}


