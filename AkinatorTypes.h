#ifndef AKINATOR_TYPES
#define AKINATOR_TYPES

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <cstddef>
#include <cstdio>

#include "TreeMemStruct/Tree.h"
#include "color_print.h"
#include "OutFunctions.h"

typedef enum
{
    AKINATOR_OK    = 1 << 0,
    MODES_NUM_ERR  = 1 << 1,
    INCORRECT_MODE = 1 << 2,
    SHOW_TREE_ERR  = 1 << 3,
    UPDATE_ERR     = 1 << 4
} AkinatorErr_t;

typedef enum
{
    DEFINITION     =  0,
    COMPARISON     =  1,
    UPDATE         =  2,
    GUESSING       =  3,
    END            =  4,
    ADD_NODE       =  5,
    SHOW_TREE      =  6,
    WRIGHT_IN_FILE =  7,
    UNKNOWN        = 10
} AkinatorMode_t;

typedef struct
{
    AkinatorMode_t num;
    const char* name;
    AkinatorErr_t (* func)(Node_t* root, int* count_img);
}mode_s;

typedef struct
{
    unsigned long hash;
    AkinatorMode_t num;
} hash_s;

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


AkinatorErr_t ShowTree(Node_t* root, int* count_img);
AkinatorErr_t SayGoodby(Node_t* root, int* count_img);
AkinatorErr_t UpdateTree(Node_t* root, int* count_img);


const mode_s ModeStructArr[] =
{
    {DEFINITION,     "определение",            NULL},
    {COMPARISON,     "сравнить",               NULL},
    {UPDATE,         "обновить",               UpdateTree},
    {GUESSING,       "отгадывание",            NULL},
    {END,            "закончить",              SayGoodby},
    {ADD_NODE,       "добавить узел",          NULL},
    {SHOW_TREE,      "показать древо знаний",  ShowTree},
    {WRIGHT_IN_FILE, "записать знания в файл", NULL},
    {UNKNOWN,        "UNKNOWN_CODE",           NULL}
};

const size_t MODES_NUM = sizeof(ModeStructArr)/sizeof(ModeStructArr[0]);

#endif // AKINATOR_TYPES
