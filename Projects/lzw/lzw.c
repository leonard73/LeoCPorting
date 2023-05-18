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
typedef struct DICTIONARY_ELEM_t
{
    uint8_t * data;
    uint32_t sz;
}DICTIONARY_ELEM;
typedef struct DICTIONARY_t
{
    DICTIONARY_ELEM * dict_data_array;
    uint32_t          sz;
}DICTIONARY;
typedef struct TMP_MATCH_POOL_t
{
    uint8_t * array_data;
    uint32_t  sz;
}TMP_MATCH_POOL;
int isFindSameInDictPool(TMP_MATCH_POOL * tmpPool_p,DICTIONARY * dict_p)
{
    uint32_t tmpPoolSZ = tmpPool_p->sz;
    for(uint32_t i=0;i<dict_p->sz;i++)
    {
        if(tmpPoolSZ == dict_p->dict_data_array[i].sz)
        {
            //start match elements
            uint8_t eq=1;
            for(uint32_t k=0;k<tmpPoolSZ;k++)
            {
                eq &= (tmpPool_p->array_data[k] ==  dict_p->dict_data_array[i].data[k]) ? 1 : 0;
            }
            if(eq==1)
            {
                //match
            }
        }
    }
}
uint32_t lzw_encode(uint8_t * input,uint8_t** output,uint32_t in_sz)
{
    uint32_t lzw_data_sz=0;
    TMP_MATCH_POOL tmpPool;
    tmpPool.sz=0,tmpPool.array_data=0;
    for(uint32_t i=0;i<in_sz;i++)
    {
        
    }
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