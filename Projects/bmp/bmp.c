#include "bmp.h"
#include <stdio.h>
#include <string.h>
void saveBmpDataToFile_colorTBL(uint8_t * data_p,uint32_t pixelW,uint32_t pixelH,uint32_t pixelS,uint8_t * colorTbl,uint32_t colorTblSize,char * path)
{
    if(colorTblSize==0) {printf("error! colorTblSize could not be zero!!!\n");return;};
    //s0 compute key parameters info
    uint32_t imageSize = pixelW * pixelH * pixelS;
    uint32_t bitsPerPixel = pixelS <<3;
    uint32_t imageDataOffsetFromHead       = 2+colorTblSize+sizeof(BMP_INFO_HEADER) + sizeof(BMP_FILE_HEADER);
    uint32_t imageDataOffsetFromFileHeader = sizeof(BMP_INFO_HEADER);
    uint32_t fileSize                      = 2+colorTblSize+imageSize+ sizeof(BMP_INFO_HEADER) + sizeof(BMP_FILE_HEADER);
    FILE * fp = fopen(path,"wb");
    //s1 save 'B' 'M'
    char Format_BM[2]={'B','M'};
    fwrite(Format_BM,sizeof(char),2,fp);
    //s2 save 12 bytes File Header
    BMP_FILE_HEADER fileHeader;
    fileHeader.bfOffBits  = imageDataOffsetFromHead;
    fileHeader.bfSize     = fileSize;
    fileHeader.bfReserved1=0;
    fileHeader.bfReserved2=0;
    fwrite(&fileHeader,sizeof(BMP_FILE_HEADER),1,fp);
    //s3 save 40 bytes Info Header
    BMP_INFO_HEADER infoHeader;
    infoHeader.biSize           = imageDataOffsetFromFileHeader;
    infoHeader.biWidth          = pixelW;
    infoHeader.biHeight         = pixelH;
    infoHeader.biPlanes         = 1;
    infoHeader.biBitCount       = bitsPerPixel;
    infoHeader.biCompression    = 0;
    infoHeader.biSizeImage      = pixelW*pixelH*1;
    infoHeader.biXPelsPerMeter  = 37795;
    infoHeader.biYPelsPerMeter  = 37795;
    infoHeader.biClrUsed        = 0;
    infoHeader.biClrImportant   = 0;
    fwrite(&infoHeader,sizeof(BMP_INFO_HEADER),1,fp);
    //s4 save color table
    fwrite(colorTbl,sizeof(uint8_t),colorTblSize,fp);
    //s5 save image data with inversed lines
    for(uint32_t i = 0;i<pixelH;i++)
    {
        fwrite(data_p+(pixelH-i-1)*(pixelW*pixelS),sizeof(uint8_t),pixelW*pixelS,fp);
    }
    fclose(fp);
}
void saveBmpDataToFile(uint8_t * data_p,uint32_t pixelW,uint32_t pixelH,uint32_t pixelS,char * path,int compLevel)
{
    //s0 compute key parameters info
    uint32_t imageSize = pixelW * pixelH * pixelS;
    uint32_t bitsPerPixel = pixelS <<3;
    uint32_t imageDataOffsetFromHead       = 2+sizeof(BMP_INFO_HEADER) + sizeof(BMP_FILE_HEADER);
    uint32_t imageDataOffsetFromFileHeader = sizeof(BMP_INFO_HEADER);
    uint32_t fileSize                      = 2+imageSize + sizeof(BMP_INFO_HEADER) + sizeof(BMP_FILE_HEADER);
    FILE * fp = fopen(path,"wb");
    //s1 save 'B' 'M'
    char Format_BM[2]={'B','M'};
    fwrite(Format_BM,sizeof(char),2,fp);
    //s2 save 12 bytes File Header
    BMP_FILE_HEADER fileHeader;
    fileHeader.bfOffBits  = imageDataOffsetFromHead;
    fileHeader.bfSize     = fileSize;
    fileHeader.bfReserved1=0;
    fileHeader.bfReserved2=0;
    fwrite(&fileHeader,sizeof(BMP_FILE_HEADER),1,fp);
    //s3 save 40 bytes Info Header
    BMP_INFO_HEADER infoHeader;
    infoHeader.biSize           = imageDataOffsetFromFileHeader;
    infoHeader.biWidth          = pixelW;
    infoHeader.biHeight         = pixelH;
    infoHeader.biPlanes         = 1;
    infoHeader.biBitCount       = bitsPerPixel;
    infoHeader.biCompression    = 0;
    infoHeader.biSizeImage      = imageSize;
    infoHeader.biXPelsPerMeter  = 37795;
    infoHeader.biYPelsPerMeter  = 37795;
    infoHeader.biClrUsed        = 0;
    infoHeader.biClrImportant   = 0;
    fwrite(&infoHeader,sizeof(BMP_INFO_HEADER),1,fp);
    //s4 save image data with inversed lines
    for(uint32_t i = 0;i<pixelH;i++)
    {
        fwrite(data_p+(pixelH-i-1)*(pixelW*pixelS),sizeof(uint8_t),pixelW*pixelS,fp);
    }
    fclose(fp);
}
void createBmpDataFromFile(BMP_DATA * bmp_data_p,char* path)
{
    // bmp_data_p = (BMP_DATA*)malloc(sizeof(BMP_DATA));
    bmp_data_p->dataPtr=0;
    readBmpFromFile(path,&bmp_data_p->bmpFileHeader,&bmp_data_p->bmpInfoHeader,&(bmp_data_p->dataPtr));
}
void releaseBmpData(BMP_DATA * bmp_data_p)
{
    if(((bmp_data_p)->dataPtr)!=0)
    {
        free((bmp_data_p)->dataPtr);
    }
}
void logBmpDataInfo(BMP_DATA * bmp_data_p)
{
    displayBmpHeader(&(bmp_data_p)->bmpFileHeader,&(bmp_data_p)->bmpInfoHeader);
}
void readBmpFromFile(char* path,BMP_FILE_HEADER * bmpFileHeader_p,BMP_INFO_HEADER * bmpInfoHeader_p,uint8_t ** dataPtr)
{
    FILE * fp = fopen(path,"rb");
    uint16_t bfType;  
    fread(&bfType,sizeof(bfType),1,fp);
    fread(bmpFileHeader_p,sizeof(BMP_FILE_HEADER),1,fp);
    fread(bmpInfoHeader_p,sizeof(BMP_INFO_HEADER),1,fp);
    size_t colorTableSize = bmpFileHeader_p->bfSize - bmpInfoHeader_p->biSizeImage -2 -sizeof(BMP_FILE_HEADER) - sizeof(BMP_INFO_HEADER);
    printf("colorTableSize=%d\n",colorTableSize);
    if(colorTableSize>0)
    {
        uint8_t * table = (uint8_t *)malloc(colorTableSize);
        fread(table,1,colorTableSize,fp);
        // for(int i=0;i<colorTableSize;i++)
        // {
        //     printf("colortable[%3d]: %3d\n",i,table[i]);
        // }
        printf("bfType: %d %d [%c%c] \n",bfType&0xff,bfType>>8,bfType&0xff,bfType>>8);
        uint32_t channel = bmpInfoHeader_p->biBitCount>>3;
        uint32_t raw_dataSize=bmpInfoHeader_p->biSizeImage;
        uint32_t raw_channel = channel;
        channel =3;
        uint32_t dataSize    =bmpInfoHeader_p->biWidth * bmpInfoHeader_p->biHeight * channel;
        *dataPtr=(uint8_t *)malloc(dataSize);
        uint8_t * raw=(uint8_t *)malloc(raw_dataSize);
        // fread(raw,sizeof(uint8_t),raw_dataSize,fp);
        for(uint32_t i=0;i<bmpInfoHeader_p->biHeight;i++)
        {
            fread((raw) + (bmpInfoHeader_p->biHeight-i-1)*bmpInfoHeader_p->biWidth*raw_channel,sizeof(uint8_t),bmpInfoHeader_p->biWidth*raw_channel,fp);
        }
        BMP_COLOR_TBL * tbl_array = (BMP_COLOR_TBL*)table;


        for(uint32_t i=0;i<bmpInfoHeader_p->biHeight;i++)
        {
            uint8_t * write_line_ptr = (*dataPtr) + (bmpInfoHeader_p->biHeight-i-1)*bmpInfoHeader_p->biWidth*channel;
            uint8_t * raw_line_ptr   = (raw) + (bmpInfoHeader_p->biHeight-i-1)*bmpInfoHeader_p->biWidth*raw_channel;
            for(uint32_t j=0;j<bmpInfoHeader_p->biWidth;j++)
            {
                uint8_t v = raw_line_ptr[j];
                BMP_COLOR_TBL tbl_array_ = tbl_array[v];
                write_line_ptr[j*channel+0] = tbl_array_.B;
                write_line_ptr[j*channel+1] = tbl_array_.G;
                write_line_ptr[j*channel+2] = tbl_array_.R;
                
            }
            // fread((*dataPtr) + (bmpInfoHeader_p->biHeight-i-1)*bmpInfoHeader_p->biWidth*channel,sizeof(uint8_t),bmpInfoHeader_p->biWidth*channel,fp);
        }
        free(raw);
        bmpInfoHeader_p->biBitCount=channel<<3;
        bmpInfoHeader_p->biSizeImage = bmpInfoHeader_p->biWidth * bmpInfoHeader_p->biHeight * channel;
        free(table);
    }
    else
    {
    printf("bfType: %d %d [%c%c] \n",bfType&0xff,bfType>>8,bfType&0xff,bfType>>8);
    uint32_t channel = bmpInfoHeader_p->biBitCount>>3;
    uint32_t dataSize=bmpInfoHeader_p->biSizeImage;
    *dataPtr=(uint8_t *)malloc(dataSize);
    for(uint32_t i=0;i<bmpInfoHeader_p->biHeight;i++)
    {
        fread((*dataPtr) + (bmpInfoHeader_p->biHeight-i-1)*bmpInfoHeader_p->biWidth*channel,sizeof(uint8_t),bmpInfoHeader_p->biWidth*channel,fp);
    }
    }
    fclose(fp);
}
void displayBmpHeader(BMP_FILE_HEADER * bmpFileHeader_p,BMP_INFO_HEADER * bmpInfoHeader_p)
{
    // printf("bmpFileHeader_p->bfType=%d\n",bmpFileHeader_p->bfType);
    printf("bmpFileHeader_p->bfSize=%d\n",bmpFileHeader_p->bfSize);
    printf("bmpFileHeader_p->bfOffBits=%d\n",bmpFileHeader_p->bfOffBits);
    printf("bmpFileHeader_p->bfReserved1=%d\n",bmpFileHeader_p->bfReserved1);
    printf("bmpFileHeader_p->bfReserved2=%d\n",bmpFileHeader_p->bfReserved2);
    printf("\n");
    printf("bmpInfoHeader_p->biSize=%d\n",bmpInfoHeader_p->biSize);
    printf("bmpInfoHeader_p->biWidth=%d\n",bmpInfoHeader_p->biWidth);
    printf("bmpInfoHeader_p->biHeight=%d\n",bmpInfoHeader_p->biHeight);
    printf("bmpInfoHeader_p->biPlanes=%d\n",bmpInfoHeader_p->biPlanes);
    printf("bmpInfoHeader_p->biBitCount=%d\n",bmpInfoHeader_p->biBitCount);
    printf("bmpInfoHeader_p->biCompression=%d\n",bmpInfoHeader_p->biCompression);
    printf("bmpInfoHeader_p->biSizeImage=%d\n",bmpInfoHeader_p->biSizeImage);
    printf("bmpInfoHeader_p->biXPelsPerMeter=%d\n",bmpInfoHeader_p->biXPelsPerMeter);
    printf("bmpInfoHeader_p->biYPelsPerMeter=%d\n",bmpInfoHeader_p->biYPelsPerMeter);
    printf("bmpInfoHeader_p->biClrUsed=%d\n",bmpInfoHeader_p->biClrUsed);
    printf("bmpInfoHeader_p->biClrImportant=%d\n",bmpInfoHeader_p->biClrImportant);
}