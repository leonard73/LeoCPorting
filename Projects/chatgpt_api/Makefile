cc:=gcc
src:=./leoJSON.c ./gptDemo.c
inc:=-I./
ccflag:=$(inc) -O3 -w -lcurl
exe:=gptDemo
json?=./demo.json
build:
		$(cc) $(src) $(ccflag) -o $(exe)
clean:
		rm -rf $(exe)
run:
		./$(exe) 