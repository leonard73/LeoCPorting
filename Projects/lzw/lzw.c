#include "lzw.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
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
void encodeNbitsArrayFromU32(uint32_t * u32_array,uint8_t* uN_array,uint32_t sz_u32,uint32_t N_bits)
{
    memset(uN_array,0x00,sz_u32*N_bits/8);
    for(uint32_t id_u32=0;id_u32<sz_u32;id_u32++)
    {
        for(uint32_t k=0;k<N_bits;k++)
        {
            uint32_t src_index_id = id_u32;
            uint32_t src_bits_id  = k;
            uint32_t dst_bytes_id = (id_u32*N_bits+k)/8;
            uint32_t dst_bits_id  = (id_u32*N_bits+k)%8;
            uN_array[dst_bytes_id] |=  (uint8_t)(((u32_array[src_index_id] >> src_bits_id)&0x00000001) << dst_bits_id);
        }
    }
}
void decodeU32bitsArrayFromMbits(uint32_t * u32_array,uint8_t* uN_array,uint32_t bytes_uN,uint32_t bytes_u32,uint32_t N_bits)
{
    memset(u32_array,0x00,bytes_u32);
    for(uint32_t bit_id=0;bit_id<bytes_uN*8;bit_id++)
    {
        uint32_t u8_byte_index   = bit_id / 8;
        uint32_t u8_bit_index    = bit_id % 8;
        uint32_t u32_index      = bit_id / N_bits;
        uint32_t u32_bit_index  = bit_id % N_bits; 
        u32_array[u32_index]   |= ((uint32_t)((uN_array[u8_byte_index] >> u8_bit_index)&0x01)) << u32_bit_index;
    }
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
    uint32_t  array_data_id;
}TMP_MATCH_POOL;
void log_tmp_match_pool(TMP_MATCH_POOL* pool)
{
    printf("TMP MATCH POOL: [%d]: ",pool->sz);
    for(int i=0;i<pool->sz;i++)
    {
        printf(" %d ",pool->array_data[i]);
    }
    printf("\n");
}
void init_default_dictionary(DICTIONARY * dict_p)
{
    dict_p->sz = 256;
    dict_p->dict_data_array = (DICTIONARY_ELEM*)malloc(256*sizeof(DICTIONARY_ELEM));
    for(int i=0;i<dict_p->sz;i++)
    {
        dict_p->dict_data_array[i].data=(uint8_t*)malloc(1);
        dict_p->dict_data_array[i].data[0] = i;
        dict_p->dict_data_array[i].sz  = 1;
    }
}
void free_dictionary(DICTIONARY * dict_p)
{
    if(dict_p->sz>0)
    {
        for(int i=0;i<256;i++)
        {
            if(dict_p->dict_data_array[i].sz>0)
            {
                free(dict_p->dict_data_array[i].data);
            }
        }
        free(dict_p->dict_data_array);
    }
}
void free_dictionary_freeAll(DICTIONARY * dict_p)
{
    if(dict_p->sz>0)
    {
        for(int i=0;i<dict_p->sz;i++)
        {
            if(dict_p->dict_data_array[i].sz>0)
            {
                free(dict_p->dict_data_array[i].data);
            }
        }
        free(dict_p->dict_data_array);
    }
}
int FindSameInDictPool(TMP_MATCH_POOL * tmpPool_p,DICTIONARY * dict_p)
{
    int id=-1;
    uint32_t tmpPoolSZ = tmpPool_p->sz;
    uint32_t find=0;
    for(uint32_t i=0;i<dict_p->sz && !find;i++)
    {
        if(tmpPoolSZ == dict_p->dict_data_array[i].sz)
        {
            //start match elements
            uint32_t eq=0;
            for(uint32_t k=0;k<(tmpPoolSZ);k++)
            {
                if(tmpPool_p->array_data[k] !=  dict_p->dict_data_array[i].data[k]) 
                {
                    break;
                }
                else
                {
                    eq++;
                }
            }
            if(eq==tmpPoolSZ)
            {
                //match
                find=1;
                id= ((int)i);
            }
        }
    }
    return id;
}
void insertDictPool(TMP_MATCH_POOL * tmpPool_p,DICTIONARY * dict_p)
{
    dict_p->sz++;
    dict_p->dict_data_array=(DICTIONARY_ELEM*)realloc( dict_p->dict_data_array,dict_p->sz*sizeof(DICTIONARY_ELEM));
    dict_p->dict_data_array[dict_p->sz-1].data = tmpPool_p->array_data;
    // dict_p->dict_data_array[dict_p->sz-1].data=(uint8_t*)malloc((tmpPool_p->sz)*sizeof(uint8_t));
    // memcpy(dict_p->dict_data_array[dict_p->sz-1].data,tmpPool_p->array_data,(tmpPool_p->sz)*sizeof(uint8_t));
    dict_p->dict_data_array[dict_p->sz-1].sz = tmpPool_p->sz;
}
void insertDictPool_memCpy(TMP_MATCH_POOL * tmpPool_p,DICTIONARY * dict_p)
{
    dict_p->sz++;
    dict_p->dict_data_array=(DICTIONARY_ELEM*)realloc( dict_p->dict_data_array,dict_p->sz*sizeof(DICTIONARY_ELEM));
    dict_p->dict_data_array[dict_p->sz-1].data = tmpPool_p->array_data;
    dict_p->dict_data_array[dict_p->sz-1].data=(uint8_t*)malloc((tmpPool_p->sz)*sizeof(uint8_t));
    memcpy(dict_p->dict_data_array[dict_p->sz-1].data,tmpPool_p->array_data,(tmpPool_p->sz)*sizeof(uint8_t));
    dict_p->dict_data_array[dict_p->sz-1].sz = tmpPool_p->sz;
}
void logDictPool(DICTIONARY * dict_p)
{
    for(int i=0;i<dict_p->sz;i++)
    {
        printf("CODE[%d]: CONTENT: | ",i);
        for(int k=0;k<dict_p->dict_data_array[i].sz;k++)
        {
            printf("%d ",dict_p->dict_data_array[i].data[k]);
        }
        printf("|\n");
    }
}
uint32_t lzw_encode(uint8_t * input,uint8_t** output,uint32_t in_sz,uint32_t * lzwBits)
{
    DICTIONARY globalDict;
    init_default_dictionary(&globalDict);
    uint32_t lzw_data_sz=0;
    TMP_MATCH_POOL tmpPool;
    uint32_t max_dict_sz = in_sz;
    uint32_t *encode_index_array=(uint32_t*)malloc(max_dict_sz*sizeof(uint32_t));
    uint32_t record_encode_sz=0;
    tmpPool.sz=0,tmpPool.array_data=input;
    for(uint32_t i=0;i<in_sz;i++)
    {
        /*S1 GEN CURRENT TMP POOL*/
        TMP_MATCH_POOL curPool = tmpPool;
        curPool.sz++;
        // log_tmp_match_pool(&curPool);
        if(FindSameInDictPool(&curPool,&globalDict)!=-1)
        {
            // printf("ROUND[%d]: findInDict!\n",i);
            tmpPool=curPool;
        }
        else
        {
            //insert dict
            // printf("ROUND[%d]: insertDict!\n",i);
            if(globalDict.sz < 10240) 
            {
                insertDictPool(&curPool,&globalDict);
            } 
            encode_index_array[record_encode_sz++]=FindSameInDictPool(&tmpPool,&globalDict);
            printf("ENCODE:[%d]\n",encode_index_array[record_encode_sz-1]);
            tmpPool.array_data=input+i;
            tmpPool.sz=1;
            // tmpPool.array_data=(uint8_t*)realloc(tmpPool.array_data,1);
            // tmpPool.sz=1;
            // tmpPool.array_data[0]=input[i];
        }
        // logDictPool(&globalDict);
    }
    {
        tmpPool.array_data=input+in_sz-1;
        tmpPool.sz=1;
        encode_index_array[record_encode_sz++]=FindSameInDictPool(&tmpPool,&globalDict);
        printf("ENCODE:[%d]\n",encode_index_array[record_encode_sz-1]);
    }
    uint32_t maxBitsDst = (int)ceilf( log2f(globalDict.sz)) ;
    printf("ENCODE SZ = %d\n",record_encode_sz);
    printf("DICT   SZ = %d; maxBits=%d\n",globalDict.sz,maxBitsDst);
    lzw_data_sz = (int)ceilf( (float)(record_encode_sz * maxBitsDst) / 8);
    printf("lzw_data_sz=%d\n",lzw_data_sz);
    *output = (uint8_t*)malloc(lzw_data_sz);
    encodeNbitsArrayFromU32(encode_index_array,*output,record_encode_sz,maxBitsDst);
    *lzwBits = maxBitsDst;
    // free(tmpPool.array_data);
    free_dictionary(&globalDict);
    free(encode_index_array);
    return lzw_data_sz;
}
int lzw_encoder(char * fileSrc,char * fileDst)
{
    uint8_t * in_data=0,*out_data=0;
    uint32_t infileSZ = read_data_from_file(&in_data,fileSrc);
    uint32_t lzwBits  = 0;
    uint32_t outfileSZ =lzw_encode(in_data,&out_data,infileSZ,&lzwBits);
    printf("read %s %d bytes!\n",fileSrc,infileSZ);
    printf("save %s %d bytes!\n",fileDst,outfileSZ);
    char *fileDst_final = (char *) malloc(strlen(fileDst)+32);
    sprintf(fileDst_final,"%s.lzwb%d",fileDst,lzwBits);
    save_data_to_file(out_data,fileDst_final,outfileSZ);
    if(!in_data)
    {
        free(in_data);
    }
    if(!out_data)
    {
        free(out_data);
    }
    free(fileDst_final);
    return 0;
}
uint32_t readLzwBitsFromPath(char * path)
{
    int start_id=0;
    for(int i=strlen(path)-1;i>=0;i--)
    {
        if(path[i] == '.')
        {
            start_id = i;
            break;
        }
    }
    if(!strcmp((char*)(path+start_id),".lzwb9"  ))
    {
        printf("lzwb9\n");
        return  9;
    }
    
}
uint32_t lzw_decode(uint32_t * input_dict_id_array,uint8_t** output,uint32_t id_size)
{
    uint32_t coder_sz=1;
    *output = (uint8_t*)malloc(coder_sz*sizeof(uint8_t));
    DICTIONARY globalDict;
    init_default_dictionary(&globalDict);
    uint32_t lzw_data_sz=0;
    TMP_MATCH_POOL tmpPool;
    uint32_t record_encode_sz=0;
    tmpPool.sz=0,tmpPool.array_data=&output[0][0];tmpPool.array_data_id=0;
    output[0][0] = (uint8_t )input_dict_id_array[0];
    uint32_t index_id=0;
    for(uint32_t id=0;id<=coder_sz;id++)
    {
        printf("id=%d;dictSz=%d;coder_sz=%d\n",id,globalDict.sz,coder_sz);
        #if 1
        //s1 if index_id < id_size ; decode to output buffer

        //s2 gen dict
        TMP_MATCH_POOL curPool = tmpPool;
        curPool.array_data     = output[0] + curPool.array_data_id;
        curPool.sz++;
        if(( FindSameInDictPool(&curPool,&globalDict))==-1)
        {
            insertDictPool_memCpy(&curPool,&globalDict);
            curPool.array_data_id = id+1;
            curPool.sz=0;
        }
        tmpPool = curPool;
        if(index_id < id_size)
        {
            uint32_t dict_id = input_dict_id_array[index_id];
            if(dict_id < globalDict.sz)
            {
                *output = (uint8_t*)realloc(*output,coder_sz+globalDict.dict_data_array[dict_id].sz);
                for(int k=0;k<globalDict.dict_data_array[dict_id].sz;k++)
                {
                    output[0][coder_sz+k] = globalDict.dict_data_array[dict_id].data[k];
                }
                coder_sz+=globalDict.dict_data_array[dict_id].sz;
            }
            else
            {
                printf("dict_id=%d\n",dict_id);
            }
            index_id++;
        }
        #else
        TMP_MATCH_POOL curPool = tmpPool;
        tmpPool.sz++;
        log_tmp_match_pool(&tmpPool);
        int ID_MATCHED = FindSameInDictPool(&curPool,&globalDict);
        printf("id=%d,ID_MATCHED=%d\n",id,ID_MATCHED);
        continue;
        if(ID_MATCHED!=-1)
        {
            int id_ = curPool.array_data_id;
            *output = (uint8_t*)realloc(*output,(coder_sz+globalDict.dict_data_array[ID_MATCHED].sz)*sizeof(uint8_t));
            curPool.array_data = *output + id_ ;
            for(int k=0;k<globalDict.dict_data_array[ID_MATCHED].sz;k++)
            {
                output[0][coder_sz++] = globalDict.dict_data_array[ID_MATCHED].data[k];
                curPool.sz++;
                if(FindSameInDictPool(&curPool,&globalDict)==-1)
                {
                    insertDictPool(&curPool,&globalDict);
                    curPool.array_data=output[0]+curPool.array_data_id + curPool.sz-1;
                    curPool.array_data_id=curPool.array_data_id+ curPool.sz-1;
                    curPool.sz=0;
                }
            }
            tmpPool = curPool; 
        }
        else
        {
            insertDictPool(&curPool,&globalDict);
            // *output = (uint8_t*)realloc(*output,(coder_sz+globalDict.dict_data_array[globalDict.sz-1].sz)*sizeof(uint8_t));
            // for(int k=0;k<globalDict.dict_data_array[globalDict.sz-1].sz;k++)
            // {
            //     output[0][coder_sz++] = globalDict.dict_data_array[globalDict.sz-1].data[k];
            // }
            tmpPool.array_data=output[0]+tmpPool.array_data_id + tmpPool.sz-1;
            tmpPool.array_data_id=tmpPool.array_data_id+ tmpPool.sz-1;
            tmpPool.sz=0;
            // {
            //     *output = (uint8_t*)realloc(*output,(coder_sz+globalDict.dict_data_array[ID_MATCHED].sz)*sizeof(uint8_t));
            //     for(int k=0;k<globalDict.dict_data_array[ID_MATCHED].sz;k++)
            //     {
            //         output[0][coder_sz] = globalDict.dict_data_array[ID_MATCHED].data[k];
            //         curPool.sz++;
            //         if( FindSameInDictPool(&curPool,&globalDict) == -1)
            //         {
            //             insertDictPool(&curPool,&globalDict);
            //             tmpPool.array_data=output[0] + coder_sz-1;
            //             tmpPool.sz=1;
            //         }
            //         else
            //         {
            //             tmpPool = curPool;
            //         }
            //     }
            // }
        }
        #endif
    }
    free_dictionary_freeAll(&globalDict);
    return coder_sz;
}
int lzw_decoder(char * fileSrc,char * fileDst)
{
    #if 0
    const uint32_t u32_sz = 1024;
    uint32_t u32_in[u32_sz],u32_out[u32_sz];
    for(int i=0;i<u32_sz;i++)
    {
        u32_in[i]=i;
    }
    uint32_t u10_sz = (uint32_t)ceilf((float)(u32_sz * 32 )/10);
    uint8_t  *u10_array = (uint8_t*)malloc(u10_sz*sizeof(uint8_t));
    encodeNbitsArrayFromU32(u32_in,u10_array,u32_sz,10);
    decodeU32bitsArrayFromMbits(u32_out,u10_array,u10_sz,u32_sz*sizeof(uint32_t),10);
    for(int i=0;i<u32_sz;i++)
    {
        printf("in[%d]=%d;out[%d]=%d\n",i,u32_in[i],i,u32_out[i]);
    }
    free(u10_array);
    #else
    uint8_t * in_data=0,*out_data=0;
    uint32_t infileSZ = read_data_from_file(&in_data,fileSrc);
    uint32_t lzwBits  = readLzwBitsFromPath(fileSrc);
    uint32_t sz_in    = infileSZ * 8/lzwBits;
    uint32_t sz_in_u32 = (uint32_t)floor((float)infileSZ*8/lzwBits);
    uint32_t *u32_in  = (uint32_t*)malloc(sz_in_u32*sizeof(uint32_t)); 
    decodeU32bitsArrayFromMbits(u32_in,in_data,infileSZ,sz_in_u32*sizeof(uint32_t) ,lzwBits);
    for(uint32_t i=0;i<sz_in_u32;i++)
    {
        printf("u32_in[%d]=%d\n",i,u32_in[i]);
    }
    uint32_t coder_sz = lzw_decode(u32_in,&out_data,sz_in_u32);
    printf("coder_sz=%d\n",coder_sz);
    for(int i=0;i<coder_sz;i++)
    {
        printf("out_data[%d]=%c\n",i,out_data[i]);
    }
    free(u32_in);
    free(out_data);
    // uint32_t lzwBits  = 0;
    // uint32_t outfileSZ =lzw_encode(in_data,&out_data,infileSZ,&lzwBits);
    // printf("read %s %d bytes!\n",fileSrc,infileSZ);
    // printf("save %s %d bytes!\n",fileDst,outfileSZ);
    // char *fileDst_final = (char *) malloc(strlen(fileDst)+32);
    // sprintf(fileDst_final,"%s.lzwb%d",fileDst,lzwBits);
    // save_data_to_file(out_data,fileDst_final,outfileSZ);
    // if(!in_data)
    // {
    //     free(in_data);
    // }
    // if(!out_data)
    // {
    //     free(out_data);
    // }
    // free(fileDst_final);
    return 0;
    #endif
}