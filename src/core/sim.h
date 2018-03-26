#ifndef __SIM_H__
#define __SIM_H__

typedef enum {
    RTN_SIM_LOOP,
    RTN_SIM_EXIT,
}RTN_SIM;


#define SIM_ARGS void *data
extern RTN_SIM sim(SIM_ARGS);
extern void init_sim(SIM_ARGS);

extern const char * sim_st_debug[];

typedef struct {
    int loop_time;
    void *app;
}SIM_CORE_ST;


#endif
