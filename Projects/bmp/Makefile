cc:=gcc
src:=./bmp.c ./testbmp.c
cc_flag:=-w 
exe:=testbmp

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