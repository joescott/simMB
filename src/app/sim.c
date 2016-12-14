#include <unistd.h>

#include "sim.h"
#include "sim_app.h"
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

SIM_DATA_ST sim_data;

void init_sim(SIM_ARGS)
{
    DATA_ST *d_st = (DATA_ST*) data;
    d_st->sim.app = &sim_data;

    SIM_CORE_ST *sim = &d_st->sim;
    SIM_DATA_ST *app = (SIM_DATA_ST*) sim->app;

    app->status = SIM_ST_IDLE;

    app->m_factor = 100;    /** pendiente desplazamiento */
    app->z_factor = 0;      /** inicio del desplazamiento */

    valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(PESO,     0));
    valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(DESP,     0));
    valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(NONE,     0));
    valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(STATUS,   0));
    valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(PESO_MAX, 7000));
    valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(INI_GRAF, 200));
}

RTN_SIM sim(SIM_ARGS)
{
    int status, peso, desp, max, min;
    DATA_ST *d_st = (DATA_ST*) data;
    SIM_CORE_ST *sim = &d_st->sim;
    SIM_DATA_ST *app = (SIM_DATA_ST*) sim->app;

    SIM_ST last_st = app->status;
    status = MBToValue(d_st->mb->mb_mapping->tab_registers, GET_MB_HR(STATUS));
    peso = MBToValue(d_st->mb->mb_mapping->tab_registers, GET_MB_HR(PESO));
    desp = MBToValue(d_st->mb->mb_mapping->tab_registers, GET_MB_HR(DESP));
    max = MBToValue(d_st->mb->mb_mapping->tab_registers, GET_MB_HR(PESO_MAX));
    min = MBToValue(d_st->mb->mb_mapping->tab_registers, GET_MB_HR(INI_GRAF));
    switch(app->status)
    {
        case SIM_ST_IDLE:
            if(status == 1)
                app->status = SIM_ST_SEARCHING;
            break;
        case SIM_ST_SEARCHING:
            valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(PESO, ++peso));
            if(peso/app->m_factor > app->z_factor)
                valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(DESP, 
                            peso/app->m_factor));
            if(status == 0)
                app->status = SIM_ST_IDLE;
            if(peso >= max)
                app->status = SIM_ST_MAX;
            break;
        case SIM_ST_MAX:
            if(status == 0)
                app->status = SIM_ST_IDLE;
            break;
    }
    if(last_st != app->status)
        debug_printf(true, "STATE: %s\n",sim_st_debug[app->status]);
    usleep(sim->loop_time);
    return RTN_SIM_LOOP;
}
