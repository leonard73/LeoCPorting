#include "lzw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum 
{
  LZW_CMD_ENCODE = 1,
  LZW_CMD_DECODE = 2  
};

void parse_input_cmd(int argc,char * argv[])
{
    int cmd_find=0;
    char * fileNameSrc=0,* fileNameDst=0;
    for(int i=1;i<argc;i++)
    {
        if(cmd_find >0 )
        {
            if(fileNameSrc==0)
            {
                size_t fileNameSz = strlen(argv[i]);
                fileNameSrc = (char *)malloc(fileNameSz+1);
                strcpy(fileNameSrc,argv[i]);
            }
            else if(fileNameDst==0)
            {
                size_t fileNameSz = strlen(argv[i]);
                fileNameDst = (char *)malloc(fileNameSz+1);
                strcpy(fileNameDst,argv[i]);
            }

        }
        if(( !strcmp("-d",argv[i]) ) | ( !strcmp("-D",argv[i]) ))
        {
            printf("DECODE CMD!\n");
            cmd_find = LZW_CMD_DECODE;
        }
        else if(( !strcmp("-e",argv[i]) ) | ( !strcmp("-E",argv[i]) ))
        {
            printf("ENCODE CMD!\n");
            cmd_find = LZW_CMD_ENCODE;
        }
    }
    if(cmd_find == 0 || fileNameSrc==0 || fileNameDst==0)
    {
        printf("NO CMD INPUT , PLEASE TYPE ./lzwDemo -e {file need to be encoded by lzw algo} or TYPE ./lzwDemo -d {file need to be decoded by lzw algo}\n");
    }
    else
    {
        switch(cmd_find)
        {
            case LZW_CMD_ENCODE:
            {
                if(!lzw_encoder(fileNameSrc,fileNameDst))
                {
                    printf("ENCODE %s INTO %s WITH LZW OK!\n",fileNameSrc,fileNameDst);
                }
            }break;
            case LZW_CMD_DECODE:
            {
                if(!lzw_decoder(fileNameSrc,fileNameDst))
                {
                    printf("DECODE %s INTO %s WITH LZW OK!\n",fileNameSrc,fileNameDst);
                }
            }break;
            default:break;
        }
    }

    if(!fileNameSrc)
    {
        free(fileNameSrc);
    }
    if(!fileNameDst)
    {
        free(fileNameDst);
    }
}
int main(int argc,char * argv[])
{
    printf("Here is lzw demo!\n");
    parse_input_cmd(argc,argv);
}