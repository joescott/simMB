#ifndef __SIM_APP_H__
#define __SIM_APP_H__

typedef enum {
    SIM_ST_IDLE,
    SIM_ST_SEARCHING,
    SIM_ST_MAX,
    NUM_SIM_ST,
}SIM_ST;

typedef struct {
    int  status;
    int  cnt;
    int  m_factor;
    int  z_factor;
}SIM_DATA_ST;

#endif
