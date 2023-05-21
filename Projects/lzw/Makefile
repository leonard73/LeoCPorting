cc:=gcc
src:=./lzw.c ./lzw_demo.c
inc:=-I./
ccflag:=$(inc) -O3 -w -lm
exe:=lzwDemo

build:
		$(cc) $(src) $(ccflag) -o $(exe)
clean:
		rm -rf $(exe)