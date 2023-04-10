#include "leoJSON.h"
#include <stdio.h>
#include <string.h>

void leoJSON_decode_json_str(char * jsonStr)
{
    char * read_ptr = (char *)jsonStr;
    int search=1;
    while(search)
    {

        if( (*read_ptr) == '\0' || (read_ptr == NULL))
        {
            search=0;
        }
        else
        {
            printf("%c\n",*read_ptr);
        }
        read_ptr++;
    }
}