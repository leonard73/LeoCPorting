cc:=gcc
leoJSON_path:=../json
src:=$(leoJSON_path)/leoJSON.c ./gptDemo.c
inc:=-I./ -I$(leoJSON_path)
ccflag:=$(inc) -O3 -w -lcurl -fpermissive 
exe:=gptDemo
json?=./demo.json
SET_OPENAI_KEY?=$(OPENAI_API_KEY)
build:
		$(cc) $(src) $(ccflag) -o $(exe)
clean:
		rm -rf $(exe)
run:
		@./$(exe) $(SET_OPENAI_KEY)