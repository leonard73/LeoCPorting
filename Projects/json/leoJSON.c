#include "leoJSON.h"
#include <stdio.h>
#include <string.h>
leoJSON_DECODE_ITEM * leoJSON_DECODE_ITEM_Create(int maxSize)
{
    leoJSON_DECODE_ITEM * dc_item_ptr = (leoJSON_DECODE_ITEM*)malloc(sizeof(leoJSON_DECODE_ITEM));
    dc_item_ptr->count=0;
    dc_item_ptr->index                = (int*)malloc(maxSize * sizeof(int));
    return dc_item_ptr;
}
leoJSON_ITEM_SOLO * leoJSON_ITEM_SOLO_Create(int maxSize)
{
    leoJSON_ITEM_SOLO * dc_item_solo_ptr = (leoJSON_ITEM_SOLO*)malloc(maxSize*sizeof(leoJSON_ITEM_SOLO));
    for(int i=0;i<maxSize;i++)
    {
        dc_item_solo_ptr[i].index_e=0;
        dc_item_solo_ptr[i].index_s=0;
    }
    return dc_item_solo_ptr;
}
void leoJSON_ITEM_SOLO_Free(leoJSON_ITEM_SOLO * dc_item_solo_ptr)
{
    free(dc_item_solo_ptr);
}
void leoJSON_DECODE_ITEM_Free(leoJSON_DECODE_ITEM * dc_item_ptr)
{
    free(dc_item_ptr->index);
    free(dc_item_ptr);
}
void leoJSON_decode_json_str(char * jsonStr)
{
    char * read_ptr = (char *)jsonStr;
    int search=1;

    printf("len=%d\n",strlen(jsonStr));
    size_t len = strlen(jsonStr);
    leoJSON_DECODE_ITEM * dc_colon   = leoJSON_DECODE_ITEM_Create(len);
    leoJSON_DECODE_ITEM * dc_brace_l = leoJSON_DECODE_ITEM_Create(len);
    leoJSON_DECODE_ITEM * dc_brace_r = leoJSON_DECODE_ITEM_Create(len);
    leoJSON_DECODE_ITEM * dc_comma   = leoJSON_DECODE_ITEM_Create(len);
    leoJSON_ITEM_SOLO   * dc_item_solo_brace  = leoJSON_ITEM_SOLO_Create(len);
    int dc_item_solo_brace_count=0;
    // leoJSON_DECODE_ITEM * dc_colon = leoJSON_DECODE_ITEM_Create(len);
    int gid=0;
    while(search)
    {

        if( (*read_ptr) == '\0' || (read_ptr == NULL))
        {
            search=0;
        }
        else
        {
            printf("%c\n",*read_ptr);
            switch((*read_ptr))
            {
                case '{':
                {
                    dc_brace_l->index[dc_brace_l->count++]=gid;
                }break;
                case '}': 
                {
                    dc_brace_r->index[dc_brace_r->count++]=gid;
                    if(dc_brace_l->count>0)
                    {
                        if(dc_brace_l->index[dc_brace_l->count-1] < gid)
                        {
                            dc_item_solo_brace[dc_item_solo_brace_count].index_e = gid;
                            dc_item_solo_brace[dc_item_solo_brace_count++].index_s = dc_brace_l->index[dc_brace_l->count-1];
                            dc_brace_l->count--;
                        }
                    }
                }break;
                case ':':
                {

                }break;
            }
        }
        gid++;
        read_ptr++;
    }
   
    for(int j=0;j<dc_item_solo_brace_count;j++)
    {
        leoJSON_item_solo_print(dc_item_solo_brace+j,jsonStr,"dc_item_solo_brace");
        int start_index = (dc_item_solo_brace+j)->index_s;
        int end_index   = (dc_item_solo_brace+j)->index_e;
        int sub_len     = end_index - start_index+1;
        leoJSON_DECODE_ITEM * sub_dc_colon   = leoJSON_DECODE_ITEM_Create(sub_len);
        leoJSON_DECODE_ITEM * sub_dc_brace_l = leoJSON_DECODE_ITEM_Create(sub_len);
        leoJSON_DECODE_ITEM * sub_dc_brace_r = leoJSON_DECODE_ITEM_Create(sub_len);
        leoJSON_DECODE_ITEM * sub_dc_comma   = leoJSON_DECODE_ITEM_Create(sub_len);
        leoJSON_DECODE_ITEM * sub_dc_dqm     = leoJSON_DECODE_ITEM_Create(sub_len);
        leoJSON_ITEM total_items[sub_len];
        int total_items_count=0;
        int skip1=0,skip2=0;
        for(int k=(start_index+1);k<end_index;k++)
        {
            char search_char = jsonStr[k];
            int skip = skip1 | (skip1 & skip2);
            switch(search_char)
            {
                case '[':
                {
                    skip1 = 1;
                }break;
                case ']':
                {
                    skip1 = 0;
                }break;
                case '{':
                {
                    skip2 = 1;
                }break;
                case '}':
                {
                    skip2 = 0;
                }break;
                case '"':
                {
                    if(!skip) 
                    {
                        sub_dc_dqm->index[sub_dc_dqm->count++]      = k;
                    }
                }break;
                case ':':
                {
                    if(!skip) 
                    {
                        sub_dc_colon->index[sub_dc_colon->count++]  = k;
                        if(sub_dc_dqm->count>=2)
                        {
                            total_items[total_items_count].label_index_s = sub_dc_dqm->index[sub_dc_dqm->count-2]+1;
                            total_items[total_items_count].label_index_e = sub_dc_dqm->index[sub_dc_dqm->count-1]-1;
                            total_items[total_items_count].value_index_s = k+1;
                            total_items[total_items_count].value_index_e = end_index-1;
                            if(total_items_count>0)
                            {
                                total_items[total_items_count-1].value_index_e = total_items[total_items_count].label_index_s-2;
                            }
                            total_items_count++;
                        }
                    }
                }break;
                case ',':
                {
                    if(!skip) 
                    {
                        sub_dc_comma->index[sub_dc_comma->count++]  = k;
                    }
                }break;
            }
        }
        //
        printf("comma: %d\n",sub_dc_comma->count);
        printf("dqm  : %d\n",sub_dc_dqm->count);
        printf("colon: %d\n",sub_dc_colon->count);
        printf("total_items_count: %d\n",total_items_count);
        leoJSON_DECODE_ITEM_Free(sub_dc_colon);
        leoJSON_DECODE_ITEM_Free(sub_dc_brace_l);
        leoJSON_DECODE_ITEM_Free(sub_dc_brace_r);
        leoJSON_DECODE_ITEM_Free(sub_dc_comma);
        leoJSON_DECODE_ITEM_Free(sub_dc_dqm);
        
        //
    }
    leoJSON_DECODE_ITEM_Free(dc_colon);
    leoJSON_DECODE_ITEM_Free(dc_brace_l);
    leoJSON_DECODE_ITEM_Free(dc_brace_r);
    leoJSON_DECODE_ITEM_Free(dc_comma);
    leoJSON_ITEM_SOLO_Free(dc_item_solo_brace);
}
void leoJSON_item_solo_print(leoJSON_ITEM_SOLO * item,char * jsonStr,char * tag)
{
        char log_str[item->index_e-item->index_s+2];
        memcpy(log_str,jsonStr+item->index_s,item->index_e-item->index_s+1);
        log_str[item->index_e-item->index_s+1]='\0';
        printf("[%s]: %s\n",tag,log_str);    
}