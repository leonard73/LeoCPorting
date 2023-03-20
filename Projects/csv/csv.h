#ifndef __CSV_H__
#define __CSV_H__
#include <stdint.h>
#define MAX_CSV_LABEL_STR_SIZE (256)
typedef struct CSV_DATA_t
{
    uint32_t rows;
    uint32_t cols;
    char **  label;
    void *   data_1d;
}CSV_DATA;
void load_csv_data_i32(CSV_DATA * csv_data_p,char * path,uint32_t isFirstRowLabel);
void load_csv_data_f32(CSV_DATA * csv_data_p,char * path,uint32_t isFirstRowLabel);
void save_csv_data_i32(CSV_DATA * csv_data_p,char * path,uint32_t isFirstRowLabel);
void release_csv_data(CSV_DATA * csv_data_p);
void read_csv_file_info_i32(CSV_DATA * csv_data_p,char * path,uint32_t isFirstRowLabel);
void read_csv_file_info_f32(CSV_DATA * csv_data_p,char * path,uint32_t isFirstRowLabel);
// void read_csv_file_info(char * path,uint32_t isFirstRowLabel);
#endif //__CSV_H__