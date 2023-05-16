#include "lzw.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
int file_size(char* filename)
{
    struct stat statbuf;
    
    stat(filename,&statbuf);
    
    int size=statbuf.st_size;
 
    return size;
}
uint32_t read_data_from_file(uint8_t** data,char* path)
{
    FILE *fp = fopen(path,"rb"); 
    int sz=file_size(path);   
    *data = (uint8_t*)malloc(sz);
    fread(*data,1,sz,fp);
    fclose(fp);
    return sz; 
}
void save_data_to_file(uint8_t* data,char* path,uint32_t sz)
{
    FILE *fp = fopen(path,"wb"); 
    fwrite(data,1,sz,fp);
    fclose(fp);
}
int lzw_encoder(char * fileSrc,char * fileDst)
{
    uint8_t * in_data=0,*out_data=0;
    uint32_t infileSZ = read_data_from_file(&in_data,fileSrc);
    uint32_t outfileSZ=infileSZ;
    out_data = (uint8_t*)malloc(outfileSZ);
    memcpy(out_data,in_data,outfileSZ);
    printf("read %s %d bytes!\n",fileSrc,infileSZ);
    printf("save %s %d bytes!\n",fileDst,outfileSZ);
    save_data_to_file(out_data,fileDst,outfileSZ);
    if(!in_data)
    {
        free(in_data);
    }
    if(!out_data)
    {
        free(out_data);
    }
    return 0;
}