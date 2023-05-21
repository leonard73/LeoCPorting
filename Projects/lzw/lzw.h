#ifndef __LZW_H__
#define __LZW_H__
#include <stdint.h>
int lzw_encoder(char * fileSrc,char * fileDst);
int lzw_decoder(char * fileSrc,char * fileDst);
#endif //__LZW_H__