cc:=gcc
BMP_FOLDER:=../bmp
HIST_FOLDER:=../histogram
src:=./gamma.c ./testGamma.c $(BMP_FOLDER)/bmp.c $(HIST_FOLDER)/hist.c
inc:=-I./ -I$(BMP_FOLDER)/ -I$(HIST_FOLDER)/
ccflag:=$(inc) -O3 -w
exe:=gammaTest
bmp_in?=./lena.bmp
build:
		$(cc) $(src) $(ccflag) -o $(exe)
clean:
		rm -rf $(exe)
run:
		./$(exe) $(bmp_in)