cc:=gcc
src:=./csv.c ./csvTest.c
inc:=-I./
ccflag:=$(inc) -O3 -w
exe:=csvTest
csv_in?=./csvData1.csv
csv_out?=./csvOut.csv

build:
		$(cc) $(src) $(ccflag) -o $(exe)
clean:
		rm -rf $(exe)
run:
		./$(exe) $(csv_in) $(csv_out)