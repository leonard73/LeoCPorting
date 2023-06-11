cc:=gcc
BMP_FOLDER:=../bmp/
src:=$(BMP_FOLDER)/bmp.c ./hist.c ./testHist.c
cc_flag:=-w -I$(BMP_FOLDER)/
exe:=testHist

build:
		$(cc) $(src) $(cc_flag) -o $(exe)
clean:
		rm -rf $(exe)
run:
		./$(exe) ./lena.bmp
test:
		make clean
		make build
		make run