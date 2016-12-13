#ifndef __SCREEEN_H__
#define __SCREEEN_H__

/**
 *  Definiciones a ser utilizadas en el formateo de pantalla 
 */
#define _SCREEN_CLEAR       "\033[2J"
#define _SCREEN_CLEAR_LINE  "\033[2K"
#define _SCREEN_CLEAR_DOWN  "\033[0J"
#define _SCREEN_HOME        "\033[0;0f"
#define _SCREEN_LINE(x,y)   "\033["#y";"#x"f"
#define _SCREEN_MR_LINE(x)  "\033["#x"C"
#define _SCREEN_ML_LINE(x)  "\033["#x"D"
#define _SCREEN_GREEN       "\033[32m"
#define _SCREEN_RED         "\033[31m"
#define _SCREEN_GREY        "\033[30m"
#define _SCREEN_BLUE        "\033[34m"
#define _SCREEN_YELL        "\033[33m"
#define _SCREEN_DFT         "\033[37m"
#define _SCREEN_BOLD        "\033[1m"
#define _SCREEN_NCHAR       "\033[0m"
#define _SCREEN_REVERSE     "\033[7m"
#define _SCREEN_DIM         "\033[2m"
#define _SCREEN_UNDERL      "\033[4m"
#define _SCREEN_BLINK       "\033[5m"


#define _LOOP_INIT()     static bool _tdloop_on = false;\
                         static struct timeval _tdloop;\
                         static int _tdloop_inc;\
                         struct timeval _tdloop_end;\
                         if(!_tdloop_on++) gettimeofday(&_tdloop, NULL);\
                         gettimeofday(&_tdloop_end, NULL);

#define _LOOP_PRINT()    printf("%8lld",timespecDiffms(&_tdloop_end,&_tdloop));

#define _GET_LOOP_INC_VAL()  _tdloop_inc
#define _SET_LOOP_INC_VAL()  _tdloop_inc += timespecDiffms(&_tdloop_end,&_tdloop)
#define _RESET_LOOP_INC_VAL()  _tdloop_inc = 0

#define _LOOP_SET()      gettimeofday(&_tdloop,NULL);

#define _PRINT_STATUS(label,ok)  {printf("%-60s",label);printf("%6s"_SCREEN_NCHAR"\n",(!(ok))?_SCREEN_GREEN"[OK]":_SCREEN_BLUE"[NONE]");}


#endif
