/* ************************************************************
 * PROGRAMMER  :    Bryce Berwald
 * FILE        :    api.cpp
 **************************************************************/

#include "../include/curl/curl.h"
#include "../include/curl/easy.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

// Created struct for allocated memory.
struct MemoryStruct {
  char *memory;
  size_t size;
};

/*************************************************************
 * Function description goes here...
 *************************************************************/
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp){
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if(!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


/*************************************************************
 * Function description goes here...
 *************************************************************/
int main(void) {

    curl_global_init(CURL_GLOBAL_ALL);   // Global Initialization

    CURL *curl = curl_easy_init();       // Initialization
    CURLcode result;
    
    MemoryStruct chunk;
 
    chunk.memory = (char*) malloc(100);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    if(!curl){
        cout << "\nError initializing curl!\n";
        return EXIT_FAILURE;
    }

    char* jsonObj = "{ \"Key1\" : \"Value1\" , \"Key2\" : \"Value2\" }"; 

    curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json; charset=utf-8");

    // Set options
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/users");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj);

    /* send all data to this function  */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    
    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    // Perform action
    result = curl_easy_perform(curl);

    // Output a message if something went wrong!
    if(result != CURLE_OK){
        cout << "\nURL Downloading issue. -- " << curl_easy_strerror(result);
    }
 
    // Cleanup
    curl_easy_cleanup(curl);

    // Global Cleanup
    curl_global_cleanup();

    return EXIT_SUCCESS;
}

