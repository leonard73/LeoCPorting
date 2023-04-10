cc:=gcc
src:=./leoJSON.c ./testLeoJSON.c
inc:=-I./
ccflag:=$(inc) -O3 -w
exe:=testLeoJSON
json?=./demo.json
build:
		$(cc) $(src) $(ccflag) -o $(exe)
clean:
		rm -rf $(exe)
run_read:
		./$(exe) read $(json)
run_save:
		./$(exe) save $(json)