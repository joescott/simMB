/**
 * utils.c
 */


#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <regex.h>

#define __USE_XOPEN
#include <time.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>


#include "utils.h"

/**
 * Serial Port
 */
void calc_serialport(const char *charaux, char *msp)
{
	int port;
	port = (sscanf(charaux,"COM%d",&port) == 1 && port > 0)?port-1:0;
	sprintf(msp,"/dev/ttyS%d",(port));
}

char calc_parity(const char *charaux)
{
	if(!strcmp(charaux,"EVEN") || !strcmp(charaux,"ODD"))
		return charaux[0];
	return 'N'; /* NONE */
}

/**
 * SHM size
 */
int getSHMSize(const char *msfilename)
{
	int memory_size = 0;
	FILE *f;
	if((f = fopen(msfilename, "r" )) != NULL)
	{
		fscanf(f, "%d\n", &memory_size);
		fclose(f);
	}
	return memory_size;
}

void intToMBTabReg(uint16_t *tab_registers,int addrs, unsigned int valor)
{
	/*LNum*/tab_registers[addrs+1]   = (uint16_t)  (valor & 0x0000FFFF);
	/*HNum*/tab_registers[addrs] 	 = (uint16_t) ((valor & 0xFFFF0000) >> 16);
}

void intToMBTabRegHMI(uint16_t *tab_registers,int addrs, unsigned int valor)
{
	/*LNum*/tab_registers[addrs-1]   = (uint16_t)  (valor & 0x0000FFFF);
	/*HNum*/tab_registers[addrs] 	 = (uint16_t) ((valor & 0xFFFF0000) >> 16);
}

void valueToMB(uint16_t *tab_registers,int addrs, unsigned int valor, int bits )
{
    switch(bits)
    {
        case 32:
            /*LNum*/tab_registers[addrs]   = (uint16_t)  (valor & 0x0000FFFF);
            /*HNum*/tab_registers[addrs+1] = (uint16_t) ((valor & 0xFFFF0000) >> 16);
            break;
        case 16:
        default:
            /*LNum*/tab_registers[addrs]   = (uint16_t)  (valor & 0x0000FFFF);
            break;
    }
}

int MBToValue(uint16_t *tab_registers,int addrs, int bits)
{
    int valor = 0;
    switch(bits)
    {
        case 32:
            /*LNum*/valor = tab_registers[addrs];
            /*HNum*/valor += (tab_registers[addrs+1] << 16);
            break;
        case 16:
        default:
            /*LNum*/valor = tab_registers[addrs];
            break;
    }
    return valor;
}


#ifdef FREEMODBUS
/*
 * Abre el puerto configurado con la velocidad y paridad dada.
 Guarda un puntero a ese puerto en xMBMMaster
 Devuelve 1 ó 0 según se pudo o no completar la operación.
*/
int abrir_puerto(xMBHandle *xMBMMaster, SERIAL_PORT_ST *serial)
{
	UCHAR MBM_SERIAL_PORT = puerto_formato_eMB(serial->puerto);
	eMBSerialParity MBM_PARIDAD = paridad_formato_eMB(serial->paridad);
	vMBPOtherDLLInit();
	if(MB_ENOERR == eMBMSerialInit( xMBMMaster, MB_RTU, MBM_SERIAL_PORT, serial->velocidad, MBM_PARIDAD ))
		return 1;
  return 0;
}
/*
 *
  Abre el puerto configurado con la velocidad y paridad dada.
  Guarda un puntero a ese puerto en xMBMMaster
  Devuelve 1 ó 0 según se pudo o no completar la operación.
*/
int cerrar_puerto(xMBHandle *xMBMMaster)
{
	if(MB_ENOERR == eMBMClose( *xMBMMaster ))
		return 1;
	return 0;
}



/**
 * Traduce un caracter que indica paridad al valor de las constantes
 * dadas por la bibliteca de ModBus
 */
eMBSerialParity paridad_formato_eMB ( const char paridad )
{
	eMBSerialParity toRet;
	if ( paridad == 'E' )
		toRet = MB_PAR_EVEN;
	else if ( paridad == 'N' )
		toRet = MB_PAR_NONE;
	else if ( paridad == 'O' )
		toRet = MB_PAR_ODD;
	else
		toRet = UNKNOWN;
	return toRet;
}
#endif

/**
 * Traduce un string que contiene el nombre del puerto a un número que
 * lo identifica.
 */
int puerto_formato_eMB ( const char* puerto )
{
	int port;
	if(sscanf(puerto,"/dev/ttyS%d",&port) == 1)
		return port;
	return 0;
}

/**
 * Math
 */
long long int atolld (const char* s)
{
    long long int toRet=0;
    int i = 0;
    int signo = 1;

    if (s[i]=='-'){
      i++;
      signo = -1;
    }

    while(s[i]!=0)
    {
      toRet*=10;
      toRet+=s[i] -48;
      i++;
    }
    return toRet*signo;
}

/**
 * Files
 */
static char * _get_filename(const char *fmt, va_list ap)
{
	char *filename=(char*)malloc(sizeof(char)*MAX_LEN_FILE_NAME);
	vsnprintf(filename, MAX_LEN_FILE_NAME, fmt, ap);
	return filename;
}


char * get_filename(const char *fmt, ...)
{
    char *rtn; 
	va_list ap;
	va_start(ap, fmt);
	rtn = _get_filename(fmt, ap);
	va_end(ap);
    return rtn;
}

bool file_exists(const char *fmt, ...)
{
	va_list ap;
	FILE *f;
	bool rtn = false;
	va_start(ap, fmt);
	char *filename= _get_filename(fmt, ap);
	va_end(ap);
	if((f = fopen(filename, "r")) != NULL)
	{
		fclose(f);
		rtn=true;
	}
	free(filename);
	return rtn;
}

bool remove_file(const char *fmt, ...)
{
	va_list ap;
	bool rtn = false;
	va_start(ap, fmt);
	char *filename= _get_filename(fmt, ap);
	va_end(ap);
	if(!remove(filename))
        rtn = true;
	free(filename);
	return rtn;
}

bool file_exists_read_int( int *value, const char *fmt, ...)
{
	va_list ap;
	FILE *f;
	bool rtn = false;
	va_start(ap, fmt);
	char *filename= _get_filename(fmt, ap);
	va_end(ap);
	if((f = fopen(filename, "r")) != NULL)
	{
		if(fscanf(f,"%d",value) == 1)
			rtn=true;
		fclose(f);
	}
	free(filename);
	return rtn;
}

bool file_exists_read_uint( uint32_t *value, const char *fmt, ...)
{
	va_list ap;
	FILE *f;
	bool rtn = false;
	va_start(ap, fmt);
	char *filename= _get_filename(fmt, ap);
	va_end(ap);
	if((f = fopen(filename, "r")) != NULL)
	{
		if(fscanf(f,"%u",value) == 1)
			rtn=true;
		fclose(f);
	}
	free(filename);
	return rtn;
}




bool file_exists_read_ints(const char *fmtv,int cv,int value[],const char *fmt,...)
{
	va_list ap;
	FILE *f;
	bool rtn = false;
	va_start(ap, fmt);
	char *filename= _get_filename(fmt, ap);
	va_end(ap);

	if((f = fopen(filename, "r")) != NULL)
	{
		if(cv == 1 && fscanf(f,fmtv,&value[0]) == cv)
			rtn=true;
		else if(cv == 2 && fscanf(f,fmtv,&value[0],&value[1]) == cv)
			rtn=true;
		else if(cv == 3 && fscanf(f,fmtv,&value[0],&value[1],&value[2]) == cv)
			rtn=true;
		fclose(f);
	}
	free(filename);
	return rtn;
}

bool file_exists_read_uint16(uint16_t *value, const char *fmt, ...)
{
    va_list ap;
    FILE *f;
    bool rtn = false;
    int val;
    va_start(ap, fmt);
    char *filename= _get_filename(fmt, ap);
    va_end(ap);
    if((f = fopen(filename, "r")) != NULL)
    {
        if(fscanf(f,"%d",&val) == 1)
        {
            rtn=true;
            *value = (uint16_t) val;
        }
        fclose(f);
    }
    free(filename);
    return rtn;
}
bool file_exists_read_uint16_t1(uint16_t *value,const char *value_fmt, const char *fmt, ...)
{
	va_list ap;
	FILE *f;
	bool rtn = false;
  int val;
	va_start(ap, fmt);
	char *filename= _get_filename(fmt, ap);
	va_end(ap);
	if((f = fopen(filename, "r")) != NULL)
	{
		if(fscanf(f,value_fmt,&val) == 1)
    {
      rtn=true;
            *value = (uint16_t) val;
    }
		fclose(f);
	}
	free(filename);
	return rtn;
}

bool file_exists_read_uint16_t2(uint16_t *value,const char *value_fmt, const char *fmt, ...)
{
	va_list ap;
	FILE *f;
	bool rtn = false;
  int val[2];
	va_start(ap, fmt);
	char *filename= _get_filename(fmt, ap);
	va_end(ap);
	if((f = fopen(filename, "r")) != NULL)
  {
    if(fscanf(f,value_fmt,&val[0],&val[1]) == 2)
    {
      rtn=true;
            value[0] = (uint16_t) val[0];
            value[1] = (uint16_t) val[1];
    }
    fclose(f);
  }
	free(filename);
	return rtn;
}

bool file_write(char *filename, const char *fmt, ...)
{
	va_list ap;
	FILE *f;
	bool rtn = false;
	if((f = fopen(filename, "w")) != NULL)
	{
		va_start(ap, fmt);
		vfprintf(f, fmt, ap);
		va_end(ap);
        fsync(fileno(f));
		fclose(f);
		rtn=true;
	}
	return rtn;
}

bool file_exists_test_str(const char *fmtv,const char *test_value,const char *fmt,...)
{
	va_list ap;
	FILE *f;
	bool rtn = false;
	va_start(ap, fmt);
	char *filename= _get_filename(fmt, ap);
	va_end(ap);
    

	if((f = fopen(filename, "r")) != NULL)
	{
        char *str_value = (char *)malloc(strlen(test_value)+1);
        char *str_format = (char *)malloc(strlen(fmtv)+10);
        char str_format_value[10];
        snprintf(str_format_value, sizeof(str_format_value), "%%%ds", strlen(test_value));
        snprintf(str_format,strlen(fmtv)+sizeof(str_format_value), fmtv, str_format_value);
		if(fscanf(f,str_format,str_value) == 1 && !strcmp(str_value, test_value))
			rtn=true;
		fclose(f);
        free(str_value);
        free(str_format);
	}
	free(filename);
	return rtn;
}


/**
 * get_file_cTime
 */
time_t
get_file_cTime(char *filename)
{
    struct stat attr;
    stat(filename, &attr);
    return attr.st_mtime;
}

/**
 * del_file_at
 */
bool
del_file_at(char *filename, unsigned int sec_diff_time )
{
    if(file_exists(filename) &&
            get_diff_time_now(get_file_cTime(filename)) > sec_diff_time )
    {
        remove(filename);
        return true;
    }
    return false;
}

/**
 * Time
 */

/**
 * Get Full time
 * YYYY-MM-DD HH:mm:ss, millisec
 */

#define MLLS_ZERO 
char* get_fulltime_mlls()
{
        static char toRet[36];
#ifndef MLLS_ZERO 
        sprintf(toRet, "%s, 0", get_fulltime());
#else
        sprintf(toRet, "%s", get_fulltime());
#endif
        return toRet;
}

/**
 * Get Full time
 * YYYY-MM-DD HH:mm:ss
 * YYYY-MM-DD HH:mm:ss, mlls ifdef MLLS_ZERO
 */
char* get_fulltime()
{
        static char toRet[36];

        struct timeval tv;
        time_t nowtime;
        struct tm *now;

        gettimeofday(&tv, NULL);
        nowtime = tv.tv_sec;
        now = localtime(&nowtime);

#ifndef MLLS_ZERO 
        sprintf(toRet, "%04d-%02d-%02d %02d:%02d:%02d", 
                    now->tm_year+1900, 
                    now->tm_mon+1, 
                    now->tm_mday, 
                    now->tm_hour, 
                    now->tm_min, 
                    now->tm_sec);
#else
            sprintf(toRet, "%04d-%02d-%02d %02d:%02d:%02d, %03d", 
                    now->tm_year+1900, 
                    now->tm_mon+1, 
                    now->tm_mday, 
                    now->tm_hour, 
                    now->tm_min, 
                    now->tm_sec,
                    (int)tv.tv_usec/1000
                    );
#endif
        return toRet;
}

int minutoActual()
{
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	return timeinfo->tm_min;
}

int minutoSiguiente()
{
	int toRet = minutoActual();
	toRet++;
	if (toRet>59) toRet = 0;
	return toRet;
}

int segundoActual()
{
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	return timeinfo->tm_sec;
}

int64_t timespecDiffms(struct timeval *timeA_p, struct timeval *timeB_p)
{
    int64_t df = ((timeA_p->tv_sec * 1000000) + timeA_p->tv_usec) -
                 ((timeB_p->tv_sec * 1000000) + timeB_p->tv_usec);
    if(df != 0) df=df/1000;
    return df;
}

double get_diff_time_now(time_t since)
{
    struct timeval now;
    gettimeofday(&now,NULL);
    return difftime(now.tv_sec,since);
}

int64_t get_diff_time_now_ms(struct timeval since, struct timeval *now)
{
    if(now == NULL)
    {
        struct timeval _now;
        gettimeofday(&_now,NULL);
        return timespecDiffms(&_now,&since);
    }

    return timespecDiffms(now,&since);
}

struct timeval get_time_now_ms()
{
    struct timeval now;
    gettimeofday(&now,NULL);
    return now;
}

time_t get_time_now()
{
    struct timeval now;
    gettimeofday(&now,NULL);
    return now.tv_sec;
}

struct tm *get_now()
{
	time_t rawtime;
	time(&rawtime);
	return localtime(&rawtime);
}

struct timeval get_time_str(const char* fmt, const char *str)
{
    struct timeval rtn;
    struct tm tm;
    strptime(str, fmt, &tm);
    rtn.tv_sec = mktime(&tm);
    return rtn;
}

char *set_time_str(const char* fmt, const char mlls_sep, struct timeval time)
{
    char tmbuf[64], buf[64];
    struct tm *timetm = localtime(&time.tv_sec);
    strftime(tmbuf, sizeof tmbuf, fmt, timetm);
    if( mlls_sep == '\0')
        return strdup(tmbuf);
    snprintf(buf, sizeof buf, "%s%c%03d", tmbuf, mlls_sep, (int)time.tv_usec/1000);
    return strdup(buf);
}

void add_time_mlls(struct timeval *time, int64_t mlls)
{
    int64_t secs = mlls/1000;
    int64_t usecs = (mlls - ( secs * 1000)) * 1000;
    int64_t secs_usec = (time->tv_usec + usecs)/1000000;
    time->tv_usec += usecs - (secs_usec * 1000000);
    time->tv_sec  += secs + secs_usec;
}

/**
 * String Processing
 */

typedef struct{
  const char * const cstr;
  const char * const hstr;
}HTML_CONVERT;
static HTML_CONVERT chtbl[]={
  {"A","&Aacute;"},
  {"E","&EACUTE;"},
  {"I","&IACUTE;"},
  {"O","&OACUTE;"},
  {"U","&UACUTE;"},
  {"N","&NTILDE;"},
  {NULL,NULL}
};


char * replaceString(
   const  char * const original,
   const  char * const pattern,
   const  char * const replacement
   ){
  const  size_t replen = strlen(replacement);
  const  size_t patlen = strlen(pattern);
  const  size_t orilen = strlen(original);

  size_t patcnt = 0;
  const char * oriptr;
  const char * patloc;

  for (oriptr = original; (patloc = strstr(oriptr, pattern));
       oriptr = patloc + patlen)
    patcnt++;
  const size_t  retlen = orilen + patcnt * (replen - patlen);
  char * const returned = (char *) malloc( sizeof(char) * (retlen + 1) );

  if (returned != NULL)
  {
    char * retptr = returned;
    for (oriptr = original; (patloc = strstr(oriptr, pattern));
         oriptr = patloc + patlen)
    {
      const size_t skplen = patloc - oriptr;
      strncpy(retptr, oriptr, skplen);
      retptr += skplen;
      strncpy(retptr, replacement, replen);
      retptr += replen;
    }
    strcpy(retptr, oriptr);
  }
  return returned;
}

char * htmlToString(const char *str)
{
  HTML_CONVERT *ch;
  char *nnewstr;
  char *newstr = strdup(str);
  for( ch=chtbl; ch->cstr != NULL; ch++)
  {
    nnewstr = replaceString(newstr, ch->hstr, ch->cstr);
    free(newstr);
    newstr=nnewstr;
  }
  return newstr;
}

int get_circular_index(int orig, int increment, int size)
{
    int delta = orig + increment;
    if(delta > 0)
        return delta % size;
    else
        return (size - (-delta % size));
}

/**
 * string Split
 */
int split(char *tk, const char *instr,char *split_str[])
{
    int i;
    char *token;
    char *istr = strdup(instr);
    char *str;
    if(instr)
        for ( i = 0,str=istr;; i++ , str = NULL ) {
            token = strtok( str, tk);
            if (token == NULL)
                break;
            else {
                split_str[i] = (char *) malloc(strlen(token)+1);
                sprintf(split_str[i],"%s", token);
                /*printf("/%s/\n",split_str[i]);*/
            }
        }
    free(istr);
    return i;
}

void split_free(int spn, char *split_str[])
{
    while(spn--)
        free(*(split_str+spn));
}

const char* get_csv_field(char* line, int num,const char *sep)
{
    const char* tok;
    for (tok = strtok(line, sep);
            tok && *tok;
            tok = strtok(NULL, ",\n"))
        if (!--num)
            return tok;
    return NULL;
}


