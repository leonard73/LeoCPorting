cc:=gcc
BMP_FOLDER:=../bmp
HIST_FOLDER:=../histogram
src:=./csv.c ./csvTest.c $(BMP_FOLDER)/bmp.c $(HIST_FOLDER)/hist.c
inc:=-I./ -I$(BMP_FOLDER)/ -I$(HIST_FOLDER)/
ccflag:=$(inc) -O3 -w
exe:=csvTest
csv_in?=./csvData1.csv
csv_out?=./csvOut.csv
bmp_in?=./lena.bmp
build:
		$(cc) $(src) $(ccflag) -o $(exe)
clean:
		rm -rf $(exe)
run:
		./$(exe) $(csv_in) $(csv_out) $(bmp_in)