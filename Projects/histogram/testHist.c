#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "hist.h"

int main(int argc,char * argv[])
{
    if(argc<2)
    {
        printf("error: usage =>  ./bmptest [bmpInputPath]\n");
    }
    char * bmpInputPath = argv[1];
    char bmpOutputPath[] = "out.bmp";

    BMP_DATA  readBmp;
    createBmpDataFromFile(&readBmp,bmpInputPath);
    logBmpDataInfo(&readBmp);    
    uint32_t imgWidth  = (&readBmp)->bmpInfoHeader.biWidth;
    uint32_t imgHeight = (&readBmp)->bmpInfoHeader.biHeight;
    uint8_t *imgPtr    = (&readBmp)->dataPtr;
    uint32_t pixelS    = (&readBmp)->bmpInfoHeader.biBitCount>>3;
    //step1 extract b g r arrays respectively, then compute histb histg histr
    uint8_t * chR_array= (uint8_t*)malloc(imgWidth*imgHeight*sizeof(uint8_t));
    uint8_t * chG_array= (uint8_t*)malloc(imgWidth*imgHeight*sizeof(uint8_t));
    uint8_t * chB_array= (uint8_t*)malloc(imgWidth*imgHeight*sizeof(uint8_t));
    
    uint32_t * hist256R  = (uint32_t*)malloc(256 * sizeof(uint32_t));
    uint32_t * hist256G  = (uint32_t*)malloc(256 * sizeof(uint32_t));
    uint32_t * hist256B  = (uint32_t*)malloc(256 * sizeof(uint32_t));
    for(int i=0;i<(imgWidth*imgHeight);i++)
    {
        chB_array[i] = imgPtr[3*i+0];
        chG_array[i] = imgPtr[3*i+1];
        chR_array[i] = imgPtr[3*i+2];
    }
    computeHist256_u8(hist256R,chR_array,imgWidth*imgHeight*sizeof(uint8_t));
    computeHist256_u8(hist256G,chG_array,imgWidth*imgHeight*sizeof(uint8_t));
    computeHist256_u8(hist256B,chB_array,imgWidth*imgHeight*sizeof(uint8_t));
    logHistN(hist256R,256,20,0);

    uint8_t bgr888_red[3]={0,0,255}, bgr888_green[3]={0,255,0}, bgr888_blue[3]={255,0,0};
    //create histgram bmp
    uint8_t * bmg_drawer = (uint8_t *)malloc(imgWidth*imgHeight*sizeof(uint8_t)*3);
    drawHistNtoBGR888(hist256R,256,bmg_drawer,imgWidth,imgHeight,bgr888_red);
    saveBmpDataToFile(bmg_drawer,imgWidth,imgHeight,pixelS,"chRHist.bmp",0);
    drawHistNtoBGR888(hist256G,256,bmg_drawer,imgWidth,imgHeight,bgr888_green);
    saveBmpDataToFile(bmg_drawer,imgWidth,imgHeight,pixelS,"chGHist.bmp",0);
    drawHistNtoBGR888(hist256B,256,bmg_drawer,imgWidth,imgHeight,bgr888_blue);
    saveBmpDataToFile(bmg_drawer,imgWidth,imgHeight,pixelS,"chBHist.bmp",0);
    //step2 accumulate hist
    for(int k=1;k<256;k++)
    {
        hist256R[k] += hist256R[k-1];
        hist256G[k] += hist256G[k-1];
        hist256B[k] += hist256B[k-1];
    }
    //create acc_histgram bmp
    drawHistNtoBGR888(hist256R,256,bmg_drawer,imgWidth,imgHeight,bgr888_red);
    saveBmpDataToFile(bmg_drawer,imgWidth,imgHeight,pixelS,"acc_chRHist.bmp",0);
    drawHistNtoBGR888(hist256G,256,bmg_drawer,imgWidth,imgHeight,bgr888_green);
    saveBmpDataToFile(bmg_drawer,imgWidth,imgHeight,pixelS,"acc_chGHist.bmp",0);
    drawHistNtoBGR888(hist256B,256,bmg_drawer,imgWidth,imgHeight,bgr888_blue);
    saveBmpDataToFile(bmg_drawer,imgWidth,imgHeight,pixelS,"acc_chBHist.bmp",0);
    //step3 create new intensity lookup table from acc histogram
    uint8_t * newLutR = (uint8_t *)malloc(256*sizeof(uint8_t));
    uint8_t * newLutG = (uint8_t *)malloc(256*sizeof(uint8_t)); 
    uint8_t * newLutB = (uint8_t *)malloc(256*sizeof(uint8_t));  
    for(int k=0;k<256;k++)
    {
        newLutR[k] = (uint8_t) (255 * hist256R[k] / (imgHeight*imgWidth));
        newLutG[k] = (uint8_t) (255 * hist256G[k] / (imgHeight*imgWidth));
        newLutB[k] = (uint8_t) (255 * hist256B[k] / (imgHeight*imgWidth));
    }
    //step4 do newlookup transformation on each channel of images
    for(uint32_t i=0;i<(imgHeight*imgWidth);i++)
    {
        uint8_t b = imgPtr[3*i+0];
        uint8_t g = imgPtr[3*i+1];
        uint8_t r = imgPtr[3*i+2];
        imgPtr[3*i+0] = newLutB[b];
        imgPtr[3*i+1] = newLutB[g];
        imgPtr[3*i+2] = newLutB[r];
    }
    saveBmpDataToFile(imgPtr,imgWidth,imgHeight,pixelS,"out_histeq.bmp",0);
    //step4 do histogram on new image
    for(int i=0;i<(imgWidth*imgHeight);i++)
    {
        chB_array[i] = imgPtr[3*i+0];
        chG_array[i] = imgPtr[3*i+1];
        chR_array[i] = imgPtr[3*i+2];
    }
    computeHist256_u8(hist256R,chR_array,imgWidth*imgHeight*sizeof(uint8_t));
    computeHist256_u8(hist256G,chG_array,imgWidth*imgHeight*sizeof(uint8_t));
    computeHist256_u8(hist256B,chB_array,imgWidth*imgHeight*sizeof(uint8_t));
    drawHistNtoBGR888(hist256R,256,bmg_drawer,imgWidth,imgHeight,bgr888_red);
    saveBmpDataToFile(bmg_drawer,imgWidth,imgHeight,pixelS,"out_histeq_chRHist.bmp",0);
    drawHistNtoBGR888(hist256G,256,bmg_drawer,imgWidth,imgHeight,bgr888_green);
    saveBmpDataToFile(bmg_drawer,imgWidth,imgHeight,pixelS,"out_histeq_chGHist.bmp",0);
    drawHistNtoBGR888(hist256B,256,bmg_drawer,imgWidth,imgHeight,bgr888_blue);
    saveBmpDataToFile(bmg_drawer,imgWidth,imgHeight,pixelS,"out_histeq_chBHist.bmp",0);  
    //step5 do acc hist on new image
    for(int k=1;k<256;k++)
    {
        hist256R[k] += hist256R[k-1];
        hist256G[k] += hist256G[k-1];
        hist256B[k] += hist256B[k-1];
    }
    //create acc_histgram bmp
    drawHistNtoBGR888(hist256R,256,bmg_drawer,imgWidth,imgHeight,bgr888_red);
    saveBmpDataToFile(bmg_drawer,imgWidth,imgHeight,pixelS,"out_histeq_acc_chRHist.bmp",0);
    drawHistNtoBGR888(hist256G,256,bmg_drawer,imgWidth,imgHeight,bgr888_green);
    saveBmpDataToFile(bmg_drawer,imgWidth,imgHeight,pixelS,"out_histeq_acc_chGHist.bmp",0);
    drawHistNtoBGR888(hist256B,256,bmg_drawer,imgWidth,imgHeight,bgr888_blue);
    saveBmpDataToFile(bmg_drawer,imgWidth,imgHeight,pixelS,"out_histeq_acc_chBHist.bmp",0);
    //finish hist tutorial, thanks
    releaseBmpData(&readBmp);
    free(newLutR);
    free(newLutG);
    free(newLutB);
    free(hist256R);
    free(hist256G);
    free(hist256B);
    free(bmg_drawer);
    free(chR_array);
    free(chG_array);
    free(chB_array);
}