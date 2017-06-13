SOC_HOME=/local/rts2/starburst_32cores
include $(SOC_HOME)/src/os/Makefile_app.inc
PROJECT=pingpong
SRC=$(wildcard *.cc)

.PHONY: all
.DEFAULT_GOAL: all

all: $(PROJECT).nc $(PROJECT).xmd
	ctags -R ./*

.PHONY: prog
prog: 
	./$(PROJECT).nc

$(PROJECT).elf: $(SRC)
