#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "leoJSON.h"
// #include "cJSON.h"
#define write_callback_state_start_response     0
#define write_callback_state_continue_response  1
#define write_callback_state_end_response       2
char *openai_key;
char global_response[1024*128];
int  global_response_offset=0;
static int write_callback_state = write_callback_state_end_response;
size_t write_callback_chat(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    static int times=0;
    size_t response_size = size * nmemb;
    char *response = malloc(response_size + 1);
    strncpy(response, ptr, response_size);
    response[response_size] = '\0';
    leoJSON_ITEM endSingal;
    if( strstr(response,(char*)"finish_reason")!=NULL )//find_item_with_label_str(response,(char*)"finish_reason",&endSingal))
    {
        memcpy(global_response+global_response_offset,ptr,response_size);
        global_response_offset+=response_size;
        write_callback_state = write_callback_state_end_response;
        {
            global_response[global_response_offset] = '\0';
            leoJSON_ITEM chatFromGPT;
            if(find_item_with_label_str(global_response,(char*)"content",&chatFromGPT))
            {
                leoJSON_ITEM finish_;
                find_item_with_label_str(global_response,(char*)"finish_reason",&finish_);
                chatFromGPT.value_index_e = finish_.label_index_s-6;
                log_leoJSON_item(chatFromGPT,global_response);
            }
            else
            {
                printf("decode failed!!!\n");
            }        
        }
    }
    else
    {
        memcpy(global_response+global_response_offset,ptr,response_size);
        global_response_offset+=response_size;
    }
    /* print the response data to the console */
    // printf("Response: %s\n", response);
    /* free allocated memory */
    usleep(1000*200);
    free(response);
    return response_size;
}
/* function to send HTTP POST request with json data */

void send_request_chat(char *data)
{
    CURL *curl;
    CURLcode res;

    /* set the API endpoint */
    curl = curl_easy_init();
    if(curl) {
        //step1 set headers{ api key , http url}
        char header_auth[1000];
        sprintf(header_auth,"Authorization: Bearer %s",openai_key);
        /* set request headers */
        struct curl_slist *headers = NULL;
        //step2 set content{ message , model ... api interface}
        char payload_send_char_message [4096];
        char messagesTemplate[]="[{\"role\": \"user\", \"content\": \"%s\"}]";
        char chatRequestTemplate[]="{\"model\":\"%s\",\"messages\":%s}";
        char message[3072];
        sprintf(message,messagesTemplate,data);
        sprintf(payload_send_char_message,chatRequestTemplate,"gpt-3.5-turbo",message);
        /* set request data */
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, header_auth);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_send_char_message);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_chat);
        /* perform the request */
        res = curl_easy_perform(curl);
        /* check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        /* cleanup */
        curl_easy_cleanup(curl);
    }
}

int main(int argc,char * argv[])
{
    if(argc<2)
    {
        printf("error please input 1 parameter as your openai key!!!\n");
        return 0;
    }
    openai_key = argv[1];
    char input[1024];

    while (1) {
        printf("Type your message: ");
        scanf("%[^\n]%*c", input);
        send_request_chat(input);
    }

    return 0;
}
