#ifndef AKINATOR
#define AKINATOR

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>

#include "TreeMemStruct/Tree.h"
#include "color_print.h"
#include "my_DGB2_hash.h"
#include "OutFunctions.h"
#include "AkinatorTypes.h"

const int START_LENGTH = 1 << 2;

/*unused*/ char* MyGetline(char* buffer);

int WorkWithHashes(hash_s* hashes);
int GetUserAns(char** UserAns);
AkinatorMode_t AnalyzeUserAns(const char* buffer, hash_s* hashes);
int BsearchCompareFunc(const void* searching_elem, const void* cur_elem);

#ifdef DEBUG
    #define ON_DEBUG(func) func
    #define DEBUG_OF(func)
#else
    #define ON_DEBUG(func)
    #define DEBUG_OF(func) func
#endif

#ifdef _WIN32
    #include "TXLib.h"
        #ifndef DEBUG
            #define CLEN_TERMINAL system("cls")
        #else
            #define CLEN_TERMINAL
        #endif
    #define VOICE_PRINT(text) TXSpeak(text)
#elif defined(__linux__)
        #ifndef DEBUG
            #define CLEN_TERMINAL system("clear")
        #else
            #define CLEN_TERMINAL
        #endif
    #define VOICE_PRINT(text) printf("%s", text)
#else
    printf("О путник, на тебе лежит злое проклятие лорда маковода, прости ,но я не могу тебе ничем помочь, уходи...\n");
#endif

#endif // AKINATOR
