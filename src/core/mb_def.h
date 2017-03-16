#ifndef __MB_DEF_H__
#define __MB_DEF_H__

/**  
 * ModBus Variables
 */

#define  VAR_REGNUM                   10000 

#define  TOTAL_REGNUM                 VAR_REGNUM 

/**
 * ModBus Address Map
 */
#define ANY_ADDRESS	 	     0xFFFFFFFF	 
#define BEGIN_ADDRESS	 	 0	 
#define VAR_ADDRESS	 	     BEGIN_ADDRESS	 
#define END_ADDRESS          VAR_ADDRESS          + VAR_REGNUM

#if ( END_ADDRESS - BEGIN_ADDRESS != TOTAL_REGNUM )
  #error "Falla en la definicion del mapa modBus"
#endif 

/** 
 * Registros ModBus 
 */
#define  UT_BITS_NB_POINTS		        0
#define  UT_INPUT_BITS_NB_POINTS	    0
#define  UT_REGISTERS_NB_POINTS	        TOTAL_REGNUM  /* Cantidad total de Registros Usados */
#define  UT_INPUT_REGISTERS_NB_POINTS   0

#endif
