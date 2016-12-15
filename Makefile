##
# simMB Makefile 
##

#Globals definitions includes 
include VERSION
include BUILD

# Binary output name
PROGNAME=simMB

# Binary output directory 
BINDIR=${CURDIR}

# Source code Directory
SRCDIR=src 

# Relative to SRCDIR
# ModBus static libs 
MODBUS_LIB=  ../libmodbus/src/.libs/modbus.o
MODBUS_LIB+= ../libmodbus/src/.libs/modbus-data.o
MODBUS_LIB+= ../libmodbus/src/.libs/modbus-rtu.o
MODBUS_LIB+= ../libmodbus/src/.libs/modbus-tcp.o
MODBUS_INCLUDE = ../../libmodbus/src

# Build number 
BUILD_NUMBER_FILE = ../BUILD

export 

all:
	make -s -C $(SRCDIR) 

clean:
	@make -s -C $(SRCDIR) clean

test:
	@make -s -C $(SRCDIR) test
