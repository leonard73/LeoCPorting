cc:=g++
THIRD_PATY_LIBS_PATH:=$(PWD)/../../thirdParty/libs/linux
ccflag:=-w -O3 -fpermissive -luvc -lusb-1.0 -L$(THIRD_PATY_LIBS_PATH)/. -I$(THIRD_PATY_LIBS_PATH)/
ccflag+=-lGL -lGLU -lglut -w -O3
# src:=./leouvc.c
src_demo:=./leouvc.c ./leouvcDemo.c
src_demo_gl:=./leouvc.c ./leouvcDemoGL.c
exe_demo:=./leouvcDemo
exe_demoGL:=./leouvcDemoGL
build:
		$(cc) $(src_demo) $(ccflag) -o $(exe_demo)
		$(cc) $(src_demo_gl) $(ccflag) -o $(exe_demoGL)
clean:
		rm -rf $(exe_demo)
		rm -rf $(exe_demoGL)
run:
		./$(exe_demo)
run_gl:
		./$(exe_demoGL)