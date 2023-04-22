#include "leoJSON.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdint.h>
void read_data_from_file(uint8_t ** mem,char * path,int *size)
{
    FILE * fp =fopen(path,"rb");
    fseek(fp, 0, SEEK_END);
    long size_file = ftell(fp);
    *mem = (uint8_t*) malloc(size_file);
    fseek(fp, 0, SEEK_SET);
    fread(*mem,sizeof(uint8_t),size_file,fp);
    *size = (int)size_file;
    fclose(fp);
}
void read_json(char * json_file_path)
{
    uint8_t * str_json_to_read=0;
    int size=0;
    read_data_from_file(&str_json_to_read,json_file_path,&size);
    printf("size=%d\n",size);
    // memset(str_json_to_read,0x44,size);
    leoJSON_decode_json_str(str_json_to_read);
    
    free(str_json_to_read);
}
int main(int argc,char * argv[])
{
    if (argc<3)
    {
        printf("error! please input 3 parameters, ./testLeoJSON read {json_file} or ./testLeoJSON save string\n");
    }    
    printf("testLeoJSON start\n");
    if(!strcmp(argv[1],"read"))
    {
        printf("read!\n");
        read_json(argv[2]);
    }else if(!strcmp(argv[1],"save"))
    {
        printf("save!\n");
    }
    return 0;
}