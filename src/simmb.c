/*
 * slavembHMI
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/shm.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>

#include "debug.h"
#include "screen.h"
#include "utils.h"

#include "simmb.h"
#include "shell.h"
#include "sim.h"
#include "funcsimmb.h"
#include "macro_utils.h"
#include "version.h"

static SLVMB_ST slvmb=
{
	.id= SERVER_ID_DFT,

	.port_baud= 9600,
	.port_parity='N',
	.port_ndbits=8,
	.port_nsbits=1,
};

static DATA_ST d_st=
{
    .debug=false,
    .debug_modbus=false,

    .sim_time = 100000,     /** msec */
    .sim_m_factor = 100,    /** pendiente desplazamiento */
    .sim_z_factor = 0,       /** iinicio del desplazamiento */
};

static uint8_t*  mb_query;
static pthread_t main_threads[2];

#define USAGE_STRING  "\n\
 Uso: %s [-h][-p port]\n\
 -h,    --help     Help \n\
 -s,    --slave    Slave ID\n\
 -p,    --port     Path Puerto Serie\n\
 -b,    --baud     BaudRate Puerto Serie\n\
 -d,    --debug    Debug\n\
 -l,    --loop     Loop time delay\n\
 -v,    --version  Imprime la version\n\
"

struct option longopts[] =
{
    { "help",         0, 0,   'h' },
    { "id", 	      1, 0,   's' },
    { "port", 	      1, 0,   'p' },
    { "baud",         1, 0,   'b' },
    { "loop",         1, 0,   'l' },
    { "debug",        0, 0,   'd' },
    { "version",      0, 0,   'v' },
    { NULL,           0, 0,    0  }
};

static void
usage (FILE *fp, int n)
{
  fprintf(fp, "\n"_EVAL(PROGNAME)"v"_EVAL(VERSION)"\n"USAGE_STRING,_EVAL(PROGNAME));
  exit (n);
}

static void print_version()
{
    printf(VERSION_FMT, 
            _EVAL(PROGNAME),
            _EVAL(VERSION),
            _EVAL(BUILD_DATE),
            _EVAL(BUILD_NUMBER)
          );
}

/**
 * Aux Functions
 */
static void slavmb_exit(int status)
{
  pthread_mutex_destroy(&d_st.mutex);
  if(mb_query)	free(mb_query);
  if(d_st.mb_mapping)  modbus_mapping_free(d_st.mb_mapping);
  if(d_st.mb_conn) 	modbus_free(d_st.mb_conn);
  exit(status);
}

static void slavmb_error(int exit, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  if(exit == EXIT_SLVMB)
    slavmb_exit(EXIT_FAILURE);
}

static void sigHandler(int signal)
{
    printf("Signal Receive [%d] End Server\n", signal);
    slavmb_exit(EXIT_SUCCESS);
}

static void initSignals()
{
    signal(SIGTERM, sigHandler);
    signal(SIGINT,  sigHandler);
}

/**
 * ModBUS
 */
int readDOCoil(DATA_ST *d_st, uint8_t *mb_query)
{
  uint16_t addr = MODBUS_GET_INT16_FROM_INT8(mb_query, MB_DATA_ADDR);
  uint16_t value = MODBUS_GET_INT16_FROM_INT8(mb_query, MB_DATA_SINGLE_VALUE);
  debug_printf(d_st->debug, "NOT IMPLEMENTED %s [%d = %d]\n",__FUNCTION__, addr, value);
  return 0;
}

int writeDOCoil(DATA_ST *d_st, uint8_t *mb_query)
{
  uint16_t addr = MODBUS_GET_INT16_FROM_INT8(mb_query, MB_DATA_ADDR);
  uint16_t value = MODBUS_GET_INT16_FROM_INT8(mb_query, MB_DATA_SINGLE_VALUE);
  debug_printf(d_st->debug, "NOT IMPLEMENTED %s [%d = %d]\n",__FUNCTION__, addr, value);
  return 0;
}

SLVMB_READ_OHR_ST slvmbreadohr_tbl[]=
{
  {ANY_ADDRESS,         get_var      }
};
#define SLVMB_READ_OHR_TBL_SIZE (sizeof(slvmbreadohr_tbl)/sizeof(SLVMB_READ_OHR_ST))

int readAOHReg(DATA_ST *d_st, uint8_t *mb_query)
{
    int rtn = 0;
    SLVMB_READ_OHR_ST *pslvmb_readohr;
    uint16_t addr = MODBUS_GET_INT16_FROM_INT8(mb_query, MB_DATA_ADDR);
    pthread_mutex_lock(&(d_st->mutex));
    for(pslvmb_readohr = slvmbreadohr_tbl;
            (pslvmb_readohr-slvmbreadohr_tbl) < SLVMB_READ_OHR_TBL_SIZE; pslvmb_readohr++ )
        if(pslvmb_readohr->addr == addr || pslvmb_readohr->addr == ANY_ADDRESS)
            rtn = (*pslvmb_readohr->proc)(d_st,mb_query);
    pthread_mutex_unlock(&(d_st->mutex));
    return rtn;
}

int writeAOHReg(DATA_ST *d_st, uint8_t *mb_query)
{
    int rtn = 0;
    uint16_t addr = MODBUS_GET_INT16_FROM_INT8(mb_query, MB_DATA_ADDR);
    uint16_t value = MODBUS_GET_INT16_FROM_INT8(mb_query, MB_DATA_SINGLE_VALUE);
    debug_printf(d_st->debug, ">%s[%d = %d]\n",__FUNCTION__, addr, value);
    pthread_mutex_lock(&(d_st->mutex));
    set_var(d_st,mb_query);
    pthread_mutex_unlock(&(d_st->mutex));
    return rtn;
}

int writeAOMHReg(DATA_ST *d_st, uint8_t *mb_query)
{
  uint16_t addr = MODBUS_GET_INT16_FROM_INT8(mb_query, MB_DATA_ADDR);
  uint16_t value = MODBUS_GET_INT16_FROM_INT8(mb_query, MB_DATA_SINGLE_VALUE);
  debug_printf(d_st->debug, "NOT IMPLEMENTED %s [%d = %d]\n",__FUNCTION__, addr, value);
  return 0;
}

/**
 *	Function Code	Action		      Name
 *	01 (01 hex)   Read		        Discrete Output Coils
 *	05 (05 hex)   Write single	  Discrete Output Coil
 *	15 (0F hex)   Write multiple	Discrete Output Coils
 *	02 (02 hex)   Read            Discrete Input Contacts
 *	04 (04 hex)   Read            Analog Input Registers
 *	03 (03 hex)   Read            Analog Output Holding Registers
 *	06 (06 hex)   Write single	  Analog Output Holding Register
 *	16 (10 hex)   Write multiple	Analog Output Holding Registers
 */
static MBFUNC_PROC_ST mbfunc_tbl[]=
{
  { 0x01, readDOCoil	  },
  { 0x05, writeDOCoil	  },
  { 0x0F, NULL		      },
  { 0x02, NULL		      },
  { 0x04, NULL		      },
  { 0x03, readAOHReg	  },
  { 0x06, writeAOHReg	  },
  { 0x10, writeAOMHReg	  }
};
#define MBFUNC_PROC_TBL_SIZE (sizeof(mbfunc_tbl)/sizeof(MBFUNC_PROC_ST))

int get_mbfunction(DATA_ST *d_st, uint8_t *mb_query)
{
  MBFUNC_PROC_ST *pfproc;
  int header_length = modbus_get_header_length(d_st->mb_conn);
  for(pfproc=mbfunc_tbl; (pfproc - mbfunc_tbl) < MBFUNC_PROC_TBL_SIZE; pfproc++)
    if(pfproc->id == mb_query[header_length] && pfproc->proc != NULL)
      return (*pfproc->proc)(d_st, mb_query);
  return 0;
}

/**
 * Threads
 */
void *main_cmd(void *data)
{
    SHELL *shell = init_shell(&getchar, &putchar, (void *) data); 
    while(1)
    {
        if(read_line(shell) == RTN_SLINE_READ_OK)
            proc_line(shell);
    }
	pthread_exit(NULL);
}

void *sim_mng(void *data)
{
    int rtn;
    DATA_ST *d_st = (DATA_ST*) data;
    d_st->sim_status = SIM_ST_IDLE;
    d_st->sim_cnt = 0;

    do
    {
        rtn = sim(data);
    }while(rtn == RTN_SIM_LOOP);
	pthread_exit(NULL);
}

/**
 *  SlaveMB Main
 */
int main(int argc, char *argv[])
{
    int rc;
    char c;

    initSignals();
    pthread_mutex_init(&d_st.mutex,NULL);

    mb_query = (uint8_t*) malloc(MODBUS_RTU_MAX_ADU_LENGTH);

    /*Las Opciones de cli reemplazan cualquier otra Properties/DB */
    while ((c = getopt_long(argc, argv, "hs:p:b:l:v", longopts, NULL)) != EOF)
        switch (c)
        {
            case 'h':     /* help */
                usage(stdout, 0);
                break;
            case 's':     /* Slave ID */
                slvmb.id = atoi(optarg);
                break;
            case 'p':     /* Port */
                strcpy(slvmb.port_tty,optarg);
                break;
            case 'b':     /* Baudrate */
                slvmb.port_baud = atoi(optarg);
                break;
            case 'l':     /* Loop */
                d_st.sim_time = atoi(optarg);
                break;
            case 'd':     /* Debug */
                d_st.debug = true;
                break;
            case 'v':     /* version */
                print_version();
                exit(EXIT_SUCCESS);
            default:
                usage(stderr, 1);
        }

    d_st.mb_conn = modbus_new_rtu(slvmb.port_tty,
            slvmb.port_baud,
            slvmb.port_parity,
            slvmb.port_ndbits,
            slvmb.port_nsbits);
    modbus_set_slave(d_st.mb_conn, slvmb.id);

    modbus_set_debug(d_st.mb_conn, d_st.debug);
    modbus_set_error_recovery(d_st.mb_conn,
            MODBUS_ERROR_RECOVERY_LINK |
            MODBUS_ERROR_RECOVERY_PROTOCOL);

    d_st.mb_mapping = modbus_mapping_new(
            UT_BITS_NB_POINTS,
            UT_INPUT_BITS_NB_POINTS,
            UT_REGISTERS_NB_POINTS,
            UT_INPUT_REGISTERS_NB_POINTS);
    if (d_st.mb_mapping == NULL)
        slavmb_error(EXIT_SLVMB, "Falla Mapeo Memoria ModBus: %s\n", modbus_strerror(errno));

    if((rc = modbus_connect(d_st.mb_conn)) < 0)
        slavmb_error(EXIT_SLVMB, "Falla Coneccion ModBus  %s\n", modbus_strerror(errno));
    else{

        init_var(&d_st,mb_query);

        pthread_create(&main_threads[1], NULL, &main_cmd,   (void *) &d_st);
        pthread_create(&main_threads[2], NULL, &sim_mng,    (void *) &d_st);
        pthread_detach(main_threads[0]);

        while(1)
        {
            if((rc = modbus_receive(d_st.mb_conn, mb_query)) == -1)
                continue;
            get_mbfunction(&d_st, mb_query);

            if(rc > 0)
                modbus_reply(d_st.mb_conn, mb_query, rc, d_st.mb_mapping);
        }
        pthread_join(main_threads[1], NULL);
    }

    slavmb_exit(EXIT_SUCCESS);
    return 0;
}
