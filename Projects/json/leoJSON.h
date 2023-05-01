#ifndef __LEO_JSON_H__
#define __LEO_JSON_H__
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct leoJSON_DECODE_ITEM_t
{
    int *index;
    int count;
}leoJSON_DECODE_ITEM;
typedef struct leoJSON_ITEM_SOLO_t
{
    int index_s;
    int index_e;
}leoJSON_ITEM_SOLO;
typedef struct leoJSON_ITEM_t
{
    int label_index_s;
    int label_index_e;
    int value_index_s;
    int value_index_e;
}leoJSON_ITEM;
void leoJSON_decode_json_str(char * jsonStr);
leoJSON_ITEM_SOLO * leoJSON_ITEM_SOLO_Create(int maxSize);
void leoJSON_ITEM_SOLO_Free(leoJSON_ITEM_SOLO * dc_item_solo_ptr);
leoJSON_DECODE_ITEM * leoJSON_DECODE_ITEM_Create(int maxSize);
void leoJSON_DECODE_ITEM_Free(leoJSON_DECODE_ITEM * dc_item_ptr);
void leoJSON_item_solo_print(leoJSON_ITEM_SOLO * item,char * jsonStr,char * tag);
void log_label_value_string(char * label,char* value,int value_size);
void log_leoJSON_item(leoJSON_ITEM leoItem,char * str);
int find_item_with_label_str(char * jsonStr,char * label_find,leoJSON_ITEM * find_item_p);
#endif //__LEO_JSON_H__