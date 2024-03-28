cc:=gcc
src1:=glShowTeapot.c
src2:=glShowRGB.c
src3:=glShowBMP.c
src4:=glShowJPEG.c
flag:=-lGL -lGLU -lglut -w -O3

exefile1:=glShowTeapot
exefile2:=glShowRGB
exefile3:=glShowBMP
exefile4:=glShowJPEG
#
BMP_FOLDER:=../bmp
src3+=$(BMP_FOLDER)/bmp.c 
flag+=-I./ -I$(BMP_FOLDER)/ 
#
THIRD_PARTY_LIB:=../../thirdParty/libs/linux
#
flag_jpeg:=-lGL -lGLU -lglut -w -O3 -ljpeg -L$(THIRD_PARTY_LIB)/. -I./ -I$(BMP_FOLDER)/  -I$(THIRD_PARTY_LIB)/ 
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
run_app4:
		./$(exefile4) ./testorig.jpg
clean:
		rm -rf $(exefile1)
		rm -rf $(exefile2)
		rm -rf $(exefile3)
		rm -rf $(exefile4)

#
build_jpeg:
		$(cc) -o $(exefile4) $(src4) $(flag_jpeg)