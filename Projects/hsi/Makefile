cc:=g++
THIRD_PATY_LIBS_PATH:=$(PWD)/../../thirdParty/libs/linux
LEO_UVC_PATH:=$(PWD)/../uvc
ccflag:=-w -O3 -fpermissive -luvc -lusb-1.0 -L$(THIRD_PATY_LIBS_PATH)/. -I$(THIRD_PATY_LIBS_PATH)/ -I$(LEO_UVC_PATH)/
ccflag+=-lGL -lGLU -lglut -w -O3
# src:=./leouvc.c
src_demo:=$(LEO_UVC_PATH)/leouvc.c $(LEO_UVC_PATH)/leouvcDemo.c
src_demo_gl:=$(LEO_UVC_PATH)/leouvc.c ./hsi_basic.c ./hsi_uvc_demo.c
exe_demo:=$(LEO_UVC_PATH)/leouvcDemo
exe_demoGL:=./hsi_uvc_demo
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