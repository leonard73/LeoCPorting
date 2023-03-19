#include "csv.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc,char * argv[])
{
    printf("This is csvTest program\n");
    if(argc<3)
    {
        printf("error usage! please {input csv_path_toRead} and {output vsc_path_toStore}\n");
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
    release_csv_data(&csv_load);
}