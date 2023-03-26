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
    free(rgb565);
}

void test_read_bmp_bgr888_and_save_tbl256(char * inBmpPath,char * outBmpPath)
{
    // B2 G3 R3
    BMP_DATA  readBmp;
    createBmpDataFromFile(&readBmp,inBmpPath);
    logBmpDataInfo(&readBmp);    
    uint32_t imgWidth  = (&readBmp)->bmpInfoHeader.biWidth;
    uint32_t imgHeight = (&readBmp)->bmpInfoHeader.biHeight;
    uint8_t *imgPtr    = (&readBmp)->dataPtr;
    uint32_t pixelS    = (&readBmp)->bmpInfoHeader.biBitCount>>3;
    uint8_t * newData  = (uint8_t*) malloc(imgWidth*imgHeight*1);
    uint8_t * colorTbl = (uint8_t*) malloc(256*sizeof(BMP_COLOR_TBL));
    BMP_COLOR_TBL * tbl = (BMP_COLOR_TBL*)colorTbl;
    for(uint32_t i=0;i<imgHeight;i++)
    {
       for(uint32_t j=0;j<imgWidth;j++)
       {
         uint8_t B=imgPtr[i*imgWidth*pixelS+j*pixelS];
         uint8_t G=imgPtr[i*imgWidth*pixelS+j*pixelS+1];
         uint8_t R=imgPtr[i*imgWidth*pixelS+j*pixelS+2];
         *(uint8_t*)&newData[i*imgWidth*1+j*1] = (B & 0xc0) | ((G >>2) & 0x38) | ((R>>5) & 0x07);
       } 
    }    
    for(int i=0;i<256;i++)
    {
        uint8_t b = ((i & 0xc0)>>6) * 255/4 ;
        uint8_t g = ((i & 0x38)>>3) * 255/8;
        uint8_t r = (i & 0x07) * 255/8;
        tbl[i].B  = b;
        tbl[i].G  = g;
        tbl[i].R  = r;
        tbl[i].A  = 0;
    }
    saveBmpDataToFile_colorTBL(newData,imgWidth,imgHeight,1,colorTbl,256*sizeof(BMP_COLOR_TBL),outBmpPath);
    releaseBmpData(&readBmp);
    free(colorTbl);

}
void test_read_bmp_bgr888_and_save_tbl256_level4(char * inBmpPath,char * outBmpPath)
{
    // B2 G3 R3
    BMP_DATA  readBmp;
    createBmpDataFromFile(&readBmp,inBmpPath);
    logBmpDataInfo(&readBmp);    
    uint32_t imgWidth  = (&readBmp)->bmpInfoHeader.biWidth;
    uint32_t imgHeight = (&readBmp)->bmpInfoHeader.biHeight;
    uint8_t *imgPtr    = (&readBmp)->dataPtr;
    uint32_t pixelS    = (&readBmp)->bmpInfoHeader.biBitCount>>3;
    uint8_t * newData  = (uint8_t*) malloc(imgWidth*imgHeight*1);
    uint8_t * colorTbl = (uint8_t*) malloc(256*sizeof(BMP_COLOR_TBL));
    BMP_COLOR_TBL * tbl = (BMP_COLOR_TBL*)colorTbl;
    for(uint32_t i=0;i<imgHeight;i++)
    {
       for(uint32_t j=0;j<imgWidth;j++)
       {
         uint8_t B=imgPtr[i*imgWidth*pixelS+j*pixelS];
         uint8_t G=imgPtr[i*imgWidth*pixelS+j*pixelS+1];
         uint8_t R=imgPtr[i*imgWidth*pixelS+j*pixelS+2];
         uint8_t quatity =(uint8_t)((float)R * 0.299 + (float)G * 0.587 + (float)B * 0.114);
        //  printf("B=%d\n",B);
         *(uint8_t*)&newData[i*imgWidth*1+j*1] = quatity;
       } 
    }    
    for(int i=0;i<32;i++)
    {
        tbl[i].B  = 0;
        tbl[i].G  = 0;
        tbl[i].R  = 0;
        tbl[i].A  = 0;
    }
    for(int i=32;i<64;i++)
    {
        tbl[i].B  = 255;
        tbl[i].G  = 0;
        tbl[i].R  = 0;
        tbl[i].A  = 0;
    }
    for(int i=64;i<128;i++)
    {
        tbl[i].B  = 0;
        tbl[i].G  = 255;
        tbl[i].R  = 0;
        tbl[i].A  = 0;
    }
    for(int i=128;i<256;i++)
    {
        tbl[i].B  = 0;
        tbl[i].G  = 0;
        tbl[i].R  = 255;
        tbl[i].A  = 0;
    }
    saveBmpDataToFile_colorTBL(newData,imgWidth,imgHeight,1,colorTbl,256*sizeof(BMP_COLOR_TBL),outBmpPath);
    releaseBmpData(&readBmp);
    free(colorTbl);

}
int main(int argc,char * argv[])
{
    if(argc<2)
    {
        printf("error: usage =>  ./bmptest [bmpInputPath]\n");
    }
    char * bmpInputPath = argv[1];
    char   bmpOutputPath[100];

    //test1 read bmp
    test_read_bmp_and_show_info(bmpInputPath);
    //test2 read & save bmp
    test_read_bmp_and_save_img_bmp(bmpInputPath,"out.bmp");
    //test3 read bgr888 bmp and save bgr555 bmp
    test_read_bmp_bgr888_and_save_bgr555(bmpInputPath,"out_bgr555.bmp");
    //test4 read bgr888 bmp and save 8b colorTable (256 color) bmp
    test_read_bmp_bgr888_and_save_tbl256(bmpInputPath,"out_tbl256.bmp");
    //test5 read bgr888 bmp and save 1b colorTable (2 color) bmp
    test_read_bmp_bgr888_and_save_tbl256_level4(bmpInputPath,"out_tbl256_level4.bmp");

}