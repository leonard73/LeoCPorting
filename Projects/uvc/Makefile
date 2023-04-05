cc:=g++
ccflag:=-w -O3 -luvc  `pkg-config --cflags --libs opencv4`
src:=./uvcUtil.c
exe:=./uvcUtil
build:
		$(cc) $(src) $(ccflag) -o $(exe)
clean:
		rm -rf $(exe)
run:
		./$(exe)