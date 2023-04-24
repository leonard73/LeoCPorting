#include <stdio.h>
#include <curl/curl.h>
#include "leoJSON.h"
size_t my_write_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    // printf("http rec[%ld]: %s\n",nmemb,(char *)ptr);
    char * str = (char*)ptr;
    leoJSON_decode_json_str(ptr);
    leoJSON_ITEM chatFromGPT=find_item_with_label_str(str,(char*)"content");
    log_leoJSON_item(chatFromGPT,str);
    return size;
    // return fwrite(ptr, size, nmemb, stream);
} 
int chat_with_gpt_once(promot_string) ;
CURL *curl;
int main()
{
    
    printf("Now you could chat with GPT!!!\n");
    while(1)
    {
    
    char promot_string[2048];
    scanf("%s",promot_string);
    chat_with_gpt_once(promot_string);
    }
    curl_global_cleanup();
}
int chat_with_gpt_once(char * promot_string) {
    
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    CURLcode res;
    char *url = "https://api.openai.com/v1/chat/completions";
    char *apikey = "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";
    char *prompt = promot_string;//"Hello?";
    char payload_template [40960];
    sprintf(payload_template,"{\"prompt\": \"%s\", \"n\":%d,\"size\":\"512x512\"}",prompt,10);
    char messagesTemplate[]="[{\"role\": \"user\", \"content\": \"%s\"}]";
    char chatRequestTemplate[]="{\"model\":\"%s\",\"messages\":%s}";
    char message[4096];
    sprintf(message,messagesTemplate,prompt);
    sprintf(payload_template,chatRequestTemplate,"gpt-3.5-turbo",message);
    // sprintf(payload, payload_template, prompt);
    
    
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Authorization: Bearer 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");
        // headers = curl_slist_append(headers, apikey);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_template);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write_func);
        char received_message[10240];
        FILE * fp ;//fopen("rec.json","wb");;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        // if (res != CURLE_OK) {
        //     fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        // }
        // printf("rec ok!\n");
        // printf("rec: %s\n",received_message);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        // fclose(fp);
        // fscanf(fp,"{\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":%s,\"%s\":\"%s\",\"%s\":{\"%s\":%s,\"%s\":%s,\"%s\":%s},\"%s\":[{\"%s\":{\"%s\":\"%s\",\"%s\":\"%s\"},\"%s\":\"%s\",\"%s\":%s}]}",
        // rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],rec[8],rec[9],rec[10],rec[11],rec[12],rec[13],rec[14],rec[15],
        // rec[16],rec[17],rec[18],rec[19],rec[20],rec[21],rec[22],rec[23],rec[24],rec[25]);
    }
    
    
    return 0;
}


// int main(void) {
//     CURL *curl;
//     CURLcode res;
//     char *url = "https://api.openai.com/v1/images/generations";
//     char *apikey = "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";
//     char *prompt = "We need a 3d cartoon picture.Once upon a time, in a dense forest, three lions who were brothers  attacked the big birds with all their might, and soon, the birds realized that they were no match for the three lions. However, the big birds were not ready to give up, and they fought back with all their strength.\
// The lions and the big birds fought fiercely for hours, but the lions refused to give up.";
//     char payload_template [9000];
//     sprintf(payload_template,"{\"prompt\": \"%s\", \"n\":%d,\"size\":\"512x512\"}",prompt,10);
//     char payload[1024];
    
//     // sprintf(payload, payload_template, prompt);
    
//     curl_global_init(CURL_GLOBAL_ALL);
//     curl = curl_easy_init();
    
//     if (curl) {
//         struct curl_slist *headers = NULL;
//         headers = curl_slist_append(headers, "Content-Type: application/json");
//         headers = curl_slist_append(headers, "Authorization: Bearer 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");
//         // headers = curl_slist_append(headers, apikey);
//         curl_easy_setopt(curl, CURLOPT_URL, url);
//         curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
//         curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_template);
//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);
//         res = curl_easy_perform(curl);
//         if (res != CURLE_OK) {
//             fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
//         }
//         printf("rec ok!\n");
//         curl_easy_cleanup(curl);
//         curl_slist_free_all(headers);
//     }
//     curl_global_cleanup();
    
//     return 0;
// }
