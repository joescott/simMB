# ---------------------------------------------------------------------------
# Makefile
# ---------------------------------------------------------------------------
#  Definitios exported from main Makefile
#  	PROGNAME
#  	VERSION
#  	BUILD_NUMBER
#  	MODBUS_LIB
#  	MODBUS_INCLUDE
# ---------------------------------------------------------------------------

BIN_DEST  = $(BINDIR)/$(PROGNAME)

BUILD_DATE=$$(date +'%Y%m%d')
INCLUDE  = -I. -I$(MODBUS_INCLUDE)
CFLAGS   = -MD -g -Wall -DNDEBUG -DTYPE_ADQ
CFLAGS   += $(INCLUDE) 
CFLAGS   += -DVERSION=$(VERSION) -DPROGNAME=$(PROGNAME)
CFLAGS   += -DBUILD_NUMBER=$(BUILD_NUMBER) -DBUILD_DATE=$(BUILD_DATE)

SRC      = debug.c utils.c main.c func_mb.c shell.c shell_cmds.c
SRC      += mb_func.c shell_buffer_opt.c hist.c

OBJS     = $(SRC:.c=.o)
DEPS     = $(OBJS:.o=.d) 

.PHONY: all clean 

all: $(OBJS)

clean:
	@echo Cleaning Core Module $(PROGNAME) - Version $(VERSION).$(BUILD_NUMBER)
	@rm -f $(DEPS)
	@rm -f $(OBJS)

# ---------------------------------------------------------------------------
# Rules for code generation
# ---------------------------------------------------------------------------
%.o: %.c
	@printf "%-40s" "$<" 
	@$(CC) $(CFLAGS) -o $@ -c $< >.log 2>&1 && echo "[OK]" || echo "[ERROR]" 
	@cat .log && rm -rf .log

# ---------------------------------------------------------------------------
# Compiler generated dependencies
# ---------------------------------------------------------------------------
-include $(DEPS)
