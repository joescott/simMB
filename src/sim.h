#ifndef __SIM_H__
#define __SIM_H__

typedef enum {
    RTN_SIM_LOOP,
    RTN_SIM_EXIT,
}RTN_SIM;

#define SIM_ARGS void *data
RTN_SIM sim(SIM_ARGS);


typedef enum {
    SIM_ST_IDLE,
    SIM_ST_SEARCHING,
    SIM_ST_MAX,
    NUM_SIM_ST,
}SIM_ST;


#endif
