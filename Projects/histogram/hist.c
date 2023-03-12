#include "hist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void computeHist256_u8( uint32_t *hist256,uint8_t * u8_array,uint32_t num)
{
    memset(hist256,0x00,sizeof(uint32_t)*256);
    for(int i=0;i<num;i++)
    {
        uint8_t pixel_v = u8_array[i];
        hist256[pixel_v]++;
    }
}


int cmp_hist_decreasing(const void* a, const void* b)
{
    return ((HIST_COMB*)a)->histVal < ((HIST_COMB*)b)->histVal;
}
int cmp_hist_increasing(const void* a, const void* b)
{
    return ((HIST_COMB*)a)->histVal > ((HIST_COMB*)b)->histVal;
}
/*
P1 histN : histogram array
P2 N:      histogram array number like hist256
P3 RankN : Number of Histigram elements need to be logged
P4 RankMode: 0----decreasing 1----increasing
*/
void logHistN(uint32_t * histN,uint32_t N,uint32_t rankN,uint32_t RankMode)
{
    void * cmp_func_ptr = RankMode ? cmp_hist_increasing : cmp_hist_decreasing;
    HIST_COMB * combHistN = (HIST_COMB*)malloc(sizeof(HIST_COMB)*N);
    for(int i=0;i<N;i++)
    {
        combHistN[i].histVal = histN[i];
        combHistN[i].id      = i;
    }
    qsort(combHistN,N,sizeof(HIST_COMB),cmp_func_ptr);
    for(int i=0;i<rankN;i++)
    {
        printf("Rand[%4d]: ID=%4d ; histVal=%6d\n",i,combHistN[i].id,combHistN[i].histVal);
    }
    free(combHistN);
}
/*
P1 : histN -- histogram array u32*
P2:  N     -- Number of histogram array
P3:  bgr   -- bgr array to store drawing image
P4:  bgr_width -- bgr array pixel width
P5:  bgr_hegiht-- bgr array pixel height
P6:  color choice -- uint8_t[3][r][g][b]
*/
void drawHistNtoBGR888(uint32_t * histN,uint32_t N,uint8_t* bgr,uint32_t bgr_width,uint32_t bgr_height,uint8_t colorRGB[3])
{
    //set background zero
    memset(bgr,0x00,bgr_width*bgr_height*3);
    //set drawer configuration
    const uint32_t pixels_perHistElem = bgr_width / (N);
    const uint32_t top_bar_rows       = bgr_height *1/9;
    const uint32_t bottom_bar_rows    = bgr_height *8/9;
    const uint32_t bar_rows           = bottom_bar_rows-top_bar_rows;
    const uint32_t start_cols         = pixels_perHistElem*0;
    const uint32_t end_cols           = pixels_perHistElem*(0+N);
    //cpt max
    uint32_t maxHistVal = histN[0];
    for(int i=1;i<N;i++)
    {
        maxHistVal = maxHistVal < histN[i] ? histN[i] : maxHistVal;
    }
    //
    for(uint32_t i=0;i<N;i++)
    {
        //draw each hist elem
        const uint32_t elem_row_start = bottom_bar_rows -  bar_rows * histN[i] / maxHistVal;
        for(uint32_t draw_rows = elem_row_start;draw_rows<bottom_bar_rows;draw_rows++)
        {
            for(uint32_t j=0+i*pixels_perHistElem;j<(start_cols+(i+1)*pixels_perHistElem);j++)
            {
                bgr[draw_rows*bgr_width*3+3*j+0]=colorRGB[0];
                bgr[draw_rows*bgr_width*3+3*j+1]=colorRGB[1];
                bgr[draw_rows*bgr_width*3+3*j+2]=colorRGB[2];
            }
        }
    }

    const uint8_t MaskBGR[3]={180,55,175};
    for(uint32_t i=0;i<N;i+=(N/4))
    {
        //draw each hist elem
        const uint32_t elem_row_start = bottom_bar_rows -  bar_rows ;
        for(uint32_t draw_rows = elem_row_start;draw_rows<bottom_bar_rows;draw_rows++)
        {
            for(uint32_t j=0+i*pixels_perHistElem;j<(start_cols+(i+1)*pixels_perHistElem);j++)
            {
                bgr[draw_rows*bgr_width*3+3*j+0]=MaskBGR[0];
                bgr[draw_rows*bgr_width*3+3*j+1]=MaskBGR[1];
                bgr[draw_rows*bgr_width*3+3*j+2]=MaskBGR[2];
            }
        }
    }
}
