#include <unistd.h>

#include "sim.h"
#include "common.h"
#include "utils.h"
#include "mb_map.h"
#include "debug.h"

static const char * sim_st_debug[]=
{
    "Espera de Inicio",
    "Buscando maximo",
    "Maximo alcanzado"
};

RTN_SIM sim(SIM_ARGS)
{
    int status, peso, desp, max, min;
    DATA_ST *d_st = (DATA_ST*) data;
    SIM_ST last_st = d_st->sim_status;
    status = MBToValue(d_st->mb->mb_mapping->tab_registers, GET_MB_HR(STATUS));
    peso = MBToValue(d_st->mb->mb_mapping->tab_registers, GET_MB_HR(PESO));
    desp = MBToValue(d_st->mb->mb_mapping->tab_registers, GET_MB_HR(DESP));
    max = MBToValue(d_st->mb->mb_mapping->tab_registers, GET_MB_HR(PESO_MAX));
    min = MBToValue(d_st->mb->mb_mapping->tab_registers, GET_MB_HR(INI_GRAF));
    switch(d_st->sim_status)
    {
        case SIM_ST_IDLE:
            if(status == 1)
                d_st->sim_status = SIM_ST_SEARCHING;
            break;
        case SIM_ST_SEARCHING:
            valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(PESO, ++peso));
            if(peso/d_st->sim_m_factor > d_st->sim_z_factor)
                valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(DESP, peso/d_st->sim_m_factor));
            if(status == 0)
                d_st->sim_status = SIM_ST_IDLE;
            if(peso >= max)
                d_st->sim_status = SIM_ST_MAX;
            break;
        case SIM_ST_MAX:
            if(status == 0)
                d_st->sim_status = SIM_ST_IDLE;
            break;
    }
    if(last_st != d_st->sim_status)
        debug_printf(true, "STATE: %s\n",sim_st_debug[d_st->sim_status]);
    usleep(d_st->sim_time);
    return RTN_SIM_LOOP;
}
