/* ************************************************************
 * PROGRAMMER  :    Bryce Berwald
 * FILE        :    apiFunctions.h
 **************************************************************/

#ifndef API_H
#define API_H

// System & User Included Libraries
#include "../include/curl/curl.h"
#include "../include/curl/easy.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

// Struct for allocated memory to be passed to the server.
struct MemoryStruct {
  char *memory;
  size_t size;
};

// Function prototypes
//static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

#endif