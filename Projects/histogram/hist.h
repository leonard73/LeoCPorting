#ifndef __HIST_H__
#define __HIST_H__
#include <stdint.h>
typedef struct HIST_COMB_t
{
    uint32_t histVal;//amount
    uint32_t id;
}HIST_COMB;
void computeHist256_u8( uint32_t *hist256,uint8_t * u8_array,uint32_t num);
void logHistN(uint32_t * histN,uint32_t N,uint32_t rankN,uint32_t RankMode);
void drawHistNtoBGR888(uint32_t * histN,uint32_t N,uint8_t* bgr,uint32_t bgr_width,uint32_t bgr_height,uint8_t colorRGB[3]);
#endif //__HIST_H__