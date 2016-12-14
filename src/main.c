/*
 * @file main.c
 *  Modbus Simulation Tool 
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
#include "common.h"
#include "shell.h"
#include "sim.h"
#include "func_mb.h"
#include "mb_func.h"
#include "macro_utils.h"
#include "version.h"

static MB_CONF_ST mb_conf=
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

    .sim_time = 100000,     /** msec */
    .sim_m_factor = 100,    /** pendiente desplazamiento */
    .sim_z_factor = 0,      /** inicio del desplazamiento */
};

static pthread_t main_threads[2];

/**
 * Program Options
 */
#define USAGE_STRING  "\n\
 Uso: %s [-h] -p port <optionals>\n\
 -p,    --port     Path serial port [MANDATORY]\n\
 -h,    --help     Help \n\
 -s,    --slave    Slave ID\n\
 -b,    --baud     Serial port baudrate\n\
 -d,    --debug    Debug\n\
 -l,    --loop     Loop time delay\n\
 -v,    --version  Print version\n\
"

static struct option longopts[] =
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

static const char * _opts = "hs:p:b:l:v";

/**
 * Print help usage
 */
static void usage(FILE *fp, int n)
{
  fprintf(fp, "\n"_EVAL(PROGNAME)"v"_EVAL(VERSION)"."_EVAL(BUILD_NUMBER)"\n"USAGE_STRING,_EVAL(PROGNAME));
  exit(n);
}

/**
 * Print version 
 */
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
static void main_exit(int status)
{
    clean_mb(d_st.mb);
    exit(status);
}

static void main_error(int exit, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    if(exit == EXIT_CODE)
        main_exit(EXIT_FAILURE);
}

/**
 * Signals Handler function
 */
static void sigHandler(int signal)
{
    printf("Signal Received [%d] - End Server\n", signal);
    main_exit(EXIT_SUCCESS);
}

/**
 * Init signal Handlers function
 */
static void initSignals()
{
    signal(SIGTERM, sigHandler);
    signal(SIGINT,  sigHandler);
}

/*
 * Threads
 */

/**
 * Command shell thread
 */
static void *main_cmd(void *data)
{
    SHELL *shell = init_shell(&getchar, &putchar, (void *) data); 
    while(1)
    {
        if(read_line(shell) == RTN_SLINE_READ_OK)
            proc_line(shell);
    }
	pthread_exit(NULL);
}

/**
 * Simulation Manager Thread
 */
static void *sim_mng(void *data)
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
 *  Main routine
 */
int main(int argc, char *argv[])
{
    int rc;
    char c;

    initSignals();

    while ((c = getopt_long(argc, argv, _opts, longopts, NULL)) != EOF)
        switch (c)
        {
            case 'h':     /* help */
                usage(stdout, 0);
                break;
            case 's':     /* Slave ID */
                mb_conf.id = atoi(optarg);
                break;
            case 'p':     /* Port */
                strcpy(mb_conf.port_tty,optarg);
                break;
            case 'b':     /* Baudrate */
                mb_conf.port_baud = atoi(optarg);
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

    /*
     * Mandatory argument
     */
    if(strlen(mb_conf.port_tty) == 0)
            usage(stderr, 1);

    if((d_st.mb = init_mb(&mb_conf)) != NULL && !connect_mb(d_st.mb))
    {
        init_var(d_st.mb);

        pthread_create(&main_threads[1], NULL, &main_cmd,   (void *) &d_st);
        pthread_create(&main_threads[2], NULL, &sim_mng,    (void *) &d_st);
        pthread_detach(main_threads[0]);

        while(1)
        {
            if((rc = modbus_receive(d_st.mb->mb_conn, d_st.mb->mb_query)) == -1)
                continue;
            get_mbfunction(d_st.mb);

            if(rc > 0)
                modbus_reply(d_st.mb->mb_conn, d_st.mb->mb_query, rc, d_st.mb->mb_mapping);
        }
        pthread_join(main_threads[1], NULL);
    }else
        main_error(EXIT_CODE, "ModBUS connection error: %s\n", modbus_strerror(errno));

    main_exit(EXIT_SUCCESS);
    return 0;
}
