#ifndef __MB_MAP__
#define __MB_MAP__

/**
 * Holding Registers
 */
#define HR_ADDR_PESO      0/**< 40001 + addr = 40001*/
#define HR_BITS_PESO      16
#define HR_ADDR_NONE      1/**< 40001 + addr = 40002*/
#define HR_BITS_NONE      16
#define HR_ADDR_STATUS    2/**< 40001 + addr = 40003*/
#define HR_BITS_STATUS    16
#define HR_ADDR_DESP      3/**< 40001 + addr = 40004*/
#define HR_BITS_DESP      16
#define HR_ADDR_PESO_MAX  5/**< 40001 + addr = 40006*/
#define HR_BITS_PESO_MAX  16
#define HR_ADDR_INI_GRAF  6/**< 40001 + addr = 40007*/
#define HR_BITS_INI_GRAF  16

#define SET_MB_HR(x,value) HR_ADDR_##x, value, HR_BITS_##x
#define GET_MB_HR(x) HR_ADDR_##x, HR_BITS_##x
#define GET_MB_HR_ADDR(x) HR_ADDR_##x + 40001

#endif
