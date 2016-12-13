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
CFLAGS      +=-DBUILD_NUMBER=$(BUILD_NUMBER)
CFLAGS      +=-DBUILD_DATE=$(BUILD_DATE)
INCLUDE  = -I. -I$(MODBUS_INCLUDE)
CFLAGS   = -MD -g -Wall -DNDEBUG -DTYPE_ADQ
CFLAGS   += $(INCLUDE) 
CFLAGS   += -DVERSION=$(VERSION) -DPROGNAME=$(PROGNAME)
CFLAGS   += -DBUILD_NUMBER=$(BUILD_NUMBER) -DBUILD_DATE=$(BUILD_DATE)
ASFLAGS  = -MD
LDFLAGS  = -lpthread -lrt -lc -lm 
LDLIBS   = -L.

SRC      = debug.c utils.c main.c func_mb.c shell.c shell_cmds.c sim.c

OBJS     = $(SRC:.c=.o)
DEPS     = $(OBJS:.o=.d) 

.PHONY: all clean 

all: $(BUILD_NUMBER_FILE) $(BIN_DEST)

$(BIN_DEST): $(OBJS)
	@printf "%-40s" "Linking $(PROGNAME) - Version $(VERSION).$(BUILD_NUMBER)"
	@$(CC) $(OBJS) $(MODBUS_LIB) $(LDLIBS) $(LDFLAGS) -o $@ >.log 2>&1 && echo "[OK]" || echo "[ERROR]"
	@cat .log && rm -rf .log

clean:
	@echo Cleaning $(PROGNAME) - Version $(VERSION).$(BUILD_NUMBER)
	@rm -f $(DEPS)
	@rm -f $(OBJS)
	@rm -f $(BIN_DEST) $(TGT).map

$(BUILD_NUMBER_FILE): $(SRC)
	@$(eval BUILD_NUMBER=$(shell echo $$(($(BUILD_NUMBER)+1))))
	@echo BUILD_NUMBER=$(BUILD_NUMBER) > $(BUILD_NUMBER_FILE)
	@echo Compiling $(PROGNAME) - Version $(VERSION).$(BUILD_NUMBER)

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
