SOC_HOME=/local/rts2/starburst_32cores
include $(SOC_HOME)/src/os/Makefile_app.inc
PROJECT=pingpong
SRC=$(wildcard *.cc)

.PHONY: all
.DEFAULT_GOAL: all

all: $(PROJECT).nc $(PROJECT).xmd
	./$(PROJECT).nc

$(PROJECT).elf: $(SRC)
