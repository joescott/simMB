# ---------------------------------------------------------------------------
# Makefile
# ---------------------------------------------------------------------------
include VERSION
PROGNAME    = simmb

BUILD_NUMBER_FILE=build.txt

CC          = gcc
INCLUDE     = -I. -I../libmodbus/src
CFLAGS      = -MD -g -Wall -DNDEBUG -DTYPE_ADQ
CFLAGS      += $(INCLUDE) 
CFLAGS      += -DVERSION=$(VERSION) -DPROGNAME=$(PROGNAME)
ASFLAGS     = -MD
LDFLAGS     = -lpthread -lrt -lc -lm 
LDLIBS      = -L.
MODBUS 		= ../libmodbus/src/.libs/modbus.o
MODBUS 		+= ../libmodbus/src/.libs/modbus-data.o
MODBUS 		+= ../libmodbus/src/.libs/modbus-rtu.o
MODBUS 		+= ../libmodbus/src/.libs/modbus-tcp.o
BINDIR      = ../

MB_SRC      = debug.c utils.c error.c simmb.c funcsimmb.c shell.c shell_cmds.c sim.c

CSRC        = $(MB_SRC) 
ASRC        = 
OBJS        = $(CSRC:.c=.o) $(ASRC:.S=.o)
DEPS        = $(OBJS:.o=.d) 
BIN         = $(PROGNAME)

.PHONY: all clean 

ifneq ($(MAKECMDGOALS),clean)
$(info Compilando $(PROGNAME) Version $(VERSION))
endif
all: $(BIN)

$(BIN): $(OBJS) $(BUILD_NUMBER_FILE)
	@printf "%-30s" "Linking $(PROGNAME)"
	@$(CC) $(OBJS) $(MODBUS) $(LDLIBS) $(LDFLAGS) -o $(BINDIR)/$@ && echo "OK"

clean:
	@rm -f $(DEPS)
	@rm -f $(OBJS)
	@rm -f $(BINDIR)/$(BIN) $(TGT).map

pack:
	./$(PROGNAME) -v > $(PROGNAME).info
	tar -zcvf $(PROGNAME)_$(VERSION).tgz $(PROGNAME) $(PROGNAME).info 
	mv $(PROGNAME)_$(VERSION).tgz $(DEST)

# ---------------------------------------------------------------------------
# rules for code generation
# ---------------------------------------------------------------------------
%.o:    %.c
	@printf "%-30s" "$<" 
	@$(CC) $(CFLAGS) -o $@ -c $< && echo "OK"

%.o:    %.S
	@$(CC) $(ASFLAGS) -o $@ -c $<
	
# ---------------------------------------------------------------------------
# Include build number rules.
include build.mak
# ---------------------------------------------------------------------------
# ---------------------------------------------------------------------------
#  # compiler generated dependencies
# ---------------------------------------------------------------------------
-include $(DEPS)
