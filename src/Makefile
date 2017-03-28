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
#STATIC= --static
BUILD_DATE=$$(date +'%Y%m%d')
INCLUDE  = -I. -I$(MODBUS_INCLUDE)
CFLAGS   = -MD -g -Wall -DNDEBUG
CFLAGS   += $(INCLUDE) 
CFLAGS   += -DVERSION=$(VERSION) -DPROGNAME=$(PROGNAME)
CFLAGS   += -DBUILD_NUMBER=$(BUILD_NUMBER) -DBUILD_DATE=$(BUILD_DATE)
ASFLAGS  = -MD
LDFLAGS  = -lpthread -lrt -lc -lm 
LDLIBS   = -L.

SRC_CORE = core
SRC_APP  = app
SRC_TEST = test
OBJS_CORE:=$(patsubst %.c,%.o,$(wildcard $(SRC_CORE)/*.c))
OBJS_APP:=$(patsubst %.c,%.o,$(wildcard $(SRC_APP)/*.c))

OBJS_CORE_TEST:=$(filter-out $(SRC_CORE)/main.o,$(OBJS_CORE))
OBJS_TEST:=$(patsubst %.c,%.o,$(wildcard $(SRC_TEST)/*.c))

.PHONY: all clean test

all: $(BUILD_NUMBER_FILE) $(BIN_DEST)

$(BIN_DEST): $(SRC_CORE) $(SRC_APP)
	@make -C core
	@make -C app
	@printf "%-40s" "Linking $(PROGNAME) - Version $(VERSION).$(BUILD_NUMBER)"
	@$(CC) $(STATIC) $(OBJS_CORE) $(OBJS_APP) $(MODBUS_LIB) $(LDLIBS) $(LDFLAGS) -o $@ >.log 2>&1 && echo "[OK]" || echo "[ERROR]"
	@cat .log && rm -rf .log

clean:
	@echo Cleaning Main $(PROGNAME) - Version $(VERSION).$(BUILD_NUMBER)
	@make -C core clean
	@make -C app clean
	@make -C test clean
	@rm -f $(BIN_DEST) tags

$(BUILD_NUMBER_FILE): $(SRC_CORE) $(SRC_APP)
	@$(eval BUILD_NUMBER=$(shell echo $$(($(BUILD_NUMBER)+1))))
	@echo BUILD_NUMBER=$(BUILD_NUMBER) > $(BUILD_NUMBER_FILE)
	@echo Compiling $(PROGNAME) - Version $(VERSION).$(BUILD_NUMBER)

test:
	@make -C test 
	@test/test
