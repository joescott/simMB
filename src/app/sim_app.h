#ifndef __SIM_APP_H__
#define __SIM_APP_H__

typedef enum {
    SIM_ST_OFF,
    SIM_ST_IDLE,
    SIM_ST_SEARCHING,
    SIM_ST_MAX,
    NUM_SIM_ST,
}SIM_ST;

typedef enum {
    SIM_CMD_ST_OFF,
    SIM_CMD_ST_ON,
    SIM_CMD_ST_START,
    SIM_CMD_ST_STOP,
    SIM_CMD_ST_NONE,
    NUM_SIM_CMD_ST,
}SIM_CMD_ST;


typedef struct {
    int  status;
    int  cmd;
    int  cnt;
    int  m_factor;
    int  z_factor;
}SIM_DATA_ST;

#endif
