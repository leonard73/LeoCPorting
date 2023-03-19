#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "csv.h"
void read_csv_file_info_i32(CSV_DATA * csv_data_p,char * path,uint32_t isFirstRowLabel)
{
    FILE * fp = fopen(path,"rb");
    uint32_t row_count=0,col_count=0,element=0;
    char scan_buf;
    //step1 calculate col number
    while(fscanf(fp,"%c",&scan_buf) > 0 )
    {
        
        if(scan_buf==',')
        {
            col_count++;
        }
        if(scan_buf=='\n')
        {
            break;
        }
    }
    col_count++;
    FILE * fp_secondLine = fp;
    //step2 return fp begin, calculate all elements
    fseek(fp,0,SEEK_SET);
    while(fscanf(fp,"%c",&scan_buf) > 0 )
    {
        if((scan_buf==',') || (scan_buf=='\n'))
        {
            element++;
        }
    }
    fseek(fp,-1,SEEK_CUR);
    if(fscanf(fp,"%c",&scan_buf) > 0)
    {
        if(scan_buf!='\n')
        {
            element++;
        }
    }
    if(isFirstRowLabel)
    {
        element-=col_count;
    }
    printf("element=%d,col_count=%d\n",element,col_count);
    //step3 malloc 1dmem for loading 2ddata
    csv_data_p->data_1d=0;
    if(element==0 || col_count==0)
    {
        csv_data_p->cols=col_count;
        csv_data_p->rows=0;
        fclose(fp);
        return;
    }
    csv_data_p->cols=col_count;
    csv_data_p->rows=element/col_count;
    csv_data_p->data_1d = (void*)malloc(element * sizeof(int32_t));
    fseek(fp,0,SEEK_SET);
    if(isFirstRowLabel)
    {
        csv_data_p->label = (char**)malloc(col_count*sizeof(char*));
        for(int c=0;c<col_count;c++)
        {
            csv_data_p->label[c] = (char*)malloc(MAX_CSV_LABEL_STR_SIZE);
        }
        
        
        uint32_t str_index=0;
        uint32_t col_store_id=0;
        char * load_label_p = csv_data_p->label[col_store_id];
        while(fscanf(fp,"%c",&scan_buf) > 0 )
        {
            csv_data_p->label[col_store_id][str_index++]  = scan_buf;
            if(scan_buf==',')
            {
                load_label_p[str_index-1] = '\0';
                col_store_id++;
                load_label_p = csv_data_p->label[col_store_id];
                str_index=0;
            }
            if(scan_buf=='\n')
            {
                load_label_p[str_index-1] = '\0';
                str_index=0;            
                break;
            }
        }
    }
    int32_t * load_ptr =  (uint32_t*) csv_data_p->data_1d;
    uint32_t load_element_nb=0;
    //now we start loading 2d csv data into 1d array
    for(uint32_t i=0;i<csv_data_p->rows;i++)
    {
        while(fscanf(fp,"%d,",load_ptr++) > 0)
        {
            load_element_nb++;
        }
        if(fscanf(fp,"%d\n",load_ptr++) > 0)
        {
            load_element_nb++;
        }
    }
    if(load_element_nb==element)
    {
        printf("loading csv data ok\n");
    }
    printf("load_element_nb=%d\n",load_element_nb);
    fclose(fp);
}
void load_csv_data_i32(CSV_DATA * csv_data_p,char * path,uint32_t isFirstRowLabel)
{
    csv_data_p->data_1d=0;
    csv_data_p->label=0;
    read_csv_file_info_i32(csv_data_p,path,isFirstRowLabel);
}
void release_csv_data(CSV_DATA * csv_data_p)
{
    if(csv_data_p->data_1d!=0)
    {
        free(csv_data_p->data_1d);
    }
    if(csv_data_p->label!=0)
    {
        for(int c=0;c<csv_data_p->cols;c++)
        {
            free(csv_data_p->label[c]);
        }
        free(csv_data_p->label);
    }
}