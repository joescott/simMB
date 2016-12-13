#ifndef __MBHMI_H__
#define __MBHMI_H__

/**
 */

/**  
 * ModBus Variables
 */

#define  MAX_NUM_VAR                  150   /* Mapeado en HMI - Maximo numero de variables cada una de 32 bits */
#define  VAR_REGNUM                   (MAX_NUM_VAR * 2) /* reg 16 bits => size = nvar * 2 */ 

#define  TOTAL_REGNUM                 VAR_REGNUM 

/**
 * ModBus Address Map
 */
#define ANY_ADDRESS	 	     0xFFFFFFFF	 
#define BEGIN_ADDRESS	 	 0	 
#define FUNC_ADDRESS	 	 BEGIN_ADDRESS	 
#define VAR_ADDRESS		 	 FUNC_ADDRESS         + FUNC_REGNUM	 
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

#define _VALIDHMI(x) (x >= 10 && x < (TOTAL_REGNUM))  /* Validacion de direccion HMI */

#endif
