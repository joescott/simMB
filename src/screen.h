#ifndef __SCREEEN_H__
#define __SCREEEN_H__

/**
 * Colors definitions
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

#endif
