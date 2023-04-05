cc:=gcc
src1:=glShowTeapot.c
src2:=glShowRGB.c
src3:=glShowBMP.c
flag:=-lGL -lGLU -lglut -w -O3
exefile1:=glShowTeapot
exefile2:=glShowRGB
exefile3:=glShowBMP
#
BMP_FOLDER:=../bmp
src3+=$(BMP_FOLDER)/bmp.c 
flag+=-I./ -I$(BMP_FOLDER)/ 
#
build:
		$(cc) -o $(exefile1) $(src1) $(flag)
		$(cc) -o $(exefile2) $(src2) $(flag)
		$(cc) -o $(exefile3) $(src3) $(flag)
run_app1:
		./$(exefile1)
run_app2:
		./$(exefile2)
run_app3:
		./$(exefile3) ./lena.bmp
clean:
		rm -rf $(exefile1)
		rm -rf $(exefile2)
		rm -rf $(exefile3)