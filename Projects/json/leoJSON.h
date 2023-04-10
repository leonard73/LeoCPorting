#ifndef __LEO_JSON_H__
#define __LEO_JSON_H__

typedef struct leoJSON_ITEM_t
{
    int index_s;
    int index_e;
}leoJSON_ITEM;
typedef struct leoJSON_ITEM_ARRAY_t
{
    int index_s;
    int index_e;
    leoJSON_ITEM * next;
}leoJSON_ITEM_ARRAY;
void leoJSON_decode_json_str(char * jsonStr);

#endif //__LEO_JSON_H__