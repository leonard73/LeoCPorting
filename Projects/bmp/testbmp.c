#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct HISTOGRAM_t
{
    uint32_t id;
    uint32_t amount;
}HISTOGRAM;
int cmp_hist_u32(const void* a, const void* b)
{
	return (((HISTOGRAM*)a)->amount) < (((HISTOGRAM*)b)->amount);
}
void test_read_bmp_and_show_info(char * inBmpPath)
{
    BMP_DATA  readBmp;
    createBmpDataFromFile(&readBmp,inBmpPath);
    logBmpDataInfo(&readBmp);
    releaseBmpData(&readBmp);
}

void test_read_bmp_and_save_img_bmp(char * inBmpPath,char * outBmpPath)
{
    BMP_DATA  readBmp;
    createBmpDataFromFile(&readBmp,inBmpPath);
    logBmpDataInfo(&readBmp);    
    uint32_t imgWidth  = (&readBmp)->bmpInfoHeader.biWidth;
    uint32_t imgHeight = (&readBmp)->bmpInfoHeader.biHeight;
    uint8_t *imgPtr    = (&readBmp)->dataPtr;
    uint32_t pixelS    = (&readBmp)->bmpInfoHeader.biBitCount>>3;
    saveBmpDataToFile(imgPtr,imgWidth,imgHeight,pixelS,outBmpPath,0);
    releaseBmpData(&readBmp);
}

void test_read_bmp_bgr888_and_save_bgr555(char * inBmpPath,char * outBmpPath)
{
    BMP_DATA  readBmp;
    createBmpDataFromFile(&readBmp,inBmpPath);
    logBmpDataInfo(&readBmp);    
    uint32_t imgWidth  = (&readBmp)->bmpInfoHeader.biWidth;
    uint32_t imgHeight = (&readBmp)->bmpInfoHeader.biHeight;
    uint8_t *imgPtr    = (&readBmp)->dataPtr;
    uint32_t pixelS    = (&readBmp)->bmpInfoHeader.biBitCount>>3;
    //encode rgb888 into rgb565
    uint32_t rgb565_pixelS = 2;
    uint8_t * rgb565 = (uint8_t*) malloc(imgWidth*imgHeight*rgb565_pixelS);
    for(uint32_t i=0;i<imgHeight;i++)
    {
       for(uint32_t j=0;j<imgWidth;j++)
       {
         uint16_t B=imgPtr[i*imgWidth*pixelS+j*pixelS];
         uint16_t G=imgPtr[i*imgWidth*pixelS+j*pixelS+1];
         uint16_t R=imgPtr[i*imgWidth*pixelS+j*pixelS+2];
         *(uint16_t*)&rgb565[i*imgWidth*rgb565_pixelS+j*rgb565_pixelS]   = (R<<7)&0x7c00 |  ((G<<2)&0x03e0) |  ((B>>3)&0x001f);
       } 
    }    
    saveBmpDataToFile(rgb565,imgWidth,imgHeight,rgb565_pixelS,outBmpPath,0);
    releaseBmpData(&readBmp);
}

int main(int argc,char * argv[])
{
    if(argc<2)
    {
        printf("error: usage =>  ./bmptest [bmpInputPath]\n");
    }
    char * bmpInputPath = argv[1];
    char   bmpOutputPath[100];

    //test1
    test_read_bmp_and_show_info(bmpInputPath);
    //test2
    test_read_bmp_and_save_img_bmp(bmpInputPath,"out.bmp");
    // //test3
    test_read_bmp_bgr888_and_save_bgr555(bmpInputPath,"out_bgr555.bmp");

}