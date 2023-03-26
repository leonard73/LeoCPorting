#ifndef __BMP_H__
#define __BMP_H__
#include <stdint.h>
typedef struct BMP_FILE_HEADER_t
{  
    uint32_t bfSize; 
    uint16_t bfReserved1; 
    uint16_t bfReserved2; 
    uint32_t bfOffBits;
}BMP_FILE_HEADER;//12 bytes
typedef struct BMP_INFO_HEADER_t
{
    uint32_t      biSize;
    int32_t       biWidth;
    int32_t       biHeight;
    uint16_t      biPlanes;
    uint16_t      biBitCount;
    uint32_t      biCompression;
    uint32_t      biSizeImage;
    int32_t       biXPelsPerMeter;
    int32_t       biYPelsPerMeter;
    uint32_t      biClrUsed;
    uint32_t      biClrImportant;
}BMP_INFO_HEADER;//40 bytes
typedef struct BMP_COLOR_TBL_t
{
    uint8_t B;
    uint8_t G;
    uint8_t R;
    uint8_t A;
}BMP_COLOR_TBL;
typedef struct BMP_DATA_t
{
    BMP_FILE_HEADER bmpFileHeader;
    BMP_INFO_HEADER bmpInfoHeader;
    uint8_t*           dataPtr;
}BMP_DATA;

void saveBmpDataToFile_colorTBL(uint8_t * data_p,uint32_t pixelW,uint32_t pixelH,uint32_t pixelS,uint8_t * colorTbl,uint32_t colorTblSize,char * path);
void createBmpDataFromFile(BMP_DATA * bmp_data_p,char* path);
void releaseBmpData(BMP_DATA * bmp_data_p);
void saveBmpDataToFile(uint8_t * data_p,uint32_t pixelW,uint32_t pixelH,uint32_t pixelS,char * path,int compLevel);
void logBmpDataInfo(BMP_DATA * bmp_data_p);
void readBmpFromFile(char* path,BMP_FILE_HEADER * bmpFileHeader_p,BMP_INFO_HEADER * bmpInfoHeader_p,uint8_t ** dataPtr);
void displayBmpHeader(BMP_FILE_HEADER * bmpFileHeader_p,BMP_INFO_HEADER * bmpInfoHeader_p);
#endif //__BMP_H__