#ifndef __TDD_H__
#define __TDD_H__

#define PRINTTEST(test) printf("TEST %d: %s ",tests_run,#test)
#define PRINTOK(test) printf("OK\n")
#define FAIL() printf("\nFailure in %s() [%d]\n", __func__, __LINE__)
#define _assert(test) do { if (!(test)) { FAIL(); return 1; } } while(0)
#define _verify(test) do { PRINTTEST(test); int r=test(); tests_run++; if(r) return r; else PRINTOK(test); } while(0)

#endif
