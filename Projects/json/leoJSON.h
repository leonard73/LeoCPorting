#ifndef __LEO_JSON_H__
#define __LEO_JSON_H__

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
    // struct leoJSON_ITEM_t * next;
    // struct leoJSON_ITEM_t * child;
}leoJSON_ITEM;
// typedef struct leoJSON_ITEM_ARRAY_t
// {
//     int index_s;
//     int index_e;
//     leoJSON_ITEM * next;
//     leoJSON_ITEM * child;
// }leoJSON_ITEM_ARRAY;
void leoJSON_decode_json_str(char * jsonStr);
leoJSON_ITEM_SOLO * leoJSON_ITEM_SOLO_Create(int maxSize);
void leoJSON_ITEM_SOLO_Free(leoJSON_ITEM_SOLO * dc_item_solo_ptr);
leoJSON_DECODE_ITEM * leoJSON_DECODE_ITEM_Create(int maxSize);
void leoJSON_DECODE_ITEM_Free(leoJSON_DECODE_ITEM * dc_item_ptr);
void leoJSON_item_solo_print(leoJSON_ITEM_SOLO * item,char * jsonStr,char * tag);
#endif //__LEO_JSON_H__