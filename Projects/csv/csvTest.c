#include "csv.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bmp.h"
#include "hist.h"
int main(int argc,char * argv[])
{
    printf("This is csvTest program\n");
    if(argc<4)
    {
        printf("error usage! please {input csv_path_toRead} and {output vsc_path_toStore} and {testbmp path}\n");
    }
    CSV_DATA csv_load;
    load_csv_data_i32(&csv_load,argv[1],1);
    uint32_t load_csv_rows = csv_load.rows;
    uint32_t load_csv_cols = csv_load.cols;
    int32_t *load_csv_array= (int32_t *)csv_load.data_1d;
    for(uint32_t j=0;j<load_csv_cols;j++)
    {
        printf(" %15s ",(csv_load.label[j]));
    }
    printf("\n");
    for(uint32_t i=0;i<load_csv_rows;i++)
    {
        for(uint32_t j=0;j<load_csv_cols;j++)
        {
            printf(" %15d ",load_csv_array[load_csv_cols*i+j]);
        }
        printf("\n");
    }
    //reverse first col & last col order
    uint32_t last_col_id = load_csv_cols-1;
    uint32_t first_col_id= 0;
    for(int i=0;i<load_csv_rows;i++)
    {
        int32_t record = load_csv_array[i*load_csv_cols + last_col_id];
        load_csv_array[i*load_csv_cols + last_col_id]  = load_csv_array[i*load_csv_cols + first_col_id];
        load_csv_array[i*load_csv_cols + first_col_id] = record;
    }
    save_csv_data_i32(&csv_load,argv[2],1);
    release_csv_data(&csv_load);

    //EXAMPLE2 read bmp bgr888 , save histgram into csv
    BMP_DATA  readBmp;
    createBmpDataFromFile(&readBmp,argv[3]);
    uint32_t imgWidth  = (&readBmp)->bmpInfoHeader.biWidth;
    uint32_t imgHeight = (&readBmp)->bmpInfoHeader.biHeight;
    uint8_t *imgPtr    = (&readBmp)->dataPtr;
    uint32_t pixelS    = (&readBmp)->bmpInfoHeader.biBitCount>>3;
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
    releaseBmpData(&readBmp);
    //save histogram
    CSV_DATA csvHist;
    csvHist.rows=256;
    csvHist.cols=3;
    char labels[][MAX_CSV_LABEL_STR_SIZE]={"HistR","HistG","HistB" };
    char *lab[3] = {labels[0],labels[1],labels[2]};
    csvHist.label = lab;
    int32_t *histrgb =(int32_t*)malloc(256*3*sizeof(int32_t));
    for(int i=0;i<256;i++)
    {
        histrgb[3*i+0] = hist256R[i];
        histrgb[3*i+1] = hist256G[i];
        histrgb[3*i+2] = hist256B[i];
    }
    csvHist.data_1d = (void*)histrgb;
    save_csv_data_i32(&csvHist,"lena_hist.csv",1);
    free(hist256R);
    free(hist256G);
    free(hist256B);
    free(chR_array);
    free(chG_array);
    free(chB_array);
    free(histrgb);
}