#ifndef AKINATOR_TYPES
#define AKINATOR_TYPES

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#include "ReadFile.h"
#include "TreeMemStruct/Tree.h"
#include "StackMemStruct/my_stack.h"
#include "color_print.h"
#include "OutFunctions.h"

typedef enum
{
    AKINATOR_OK    = 1 << 0,
    MODES_NUM_ERR  = 1 << 1,
    INCORRECT_MODE = 1 << 2,
    SHOW_TREE_ERR  = 1 << 3,
    UPDATE_ERR     = 1 << 4,
    GET_ANS_ERR    = 1 << 5,
    OPEN_FILE_ERR  = 1 << 6,
    MEM_ALLOC_ERR  = 1 << 7,
    READ_FILE_ERR  = 1 << 8
} AkinatorErr_t;

typedef enum
{
    DEFINITION     =  0,
    COMPARISON     =  1,
    UPDATE         =  2,
    GUESSING       =  3,
    END            =  4,
    SHOW_TREE      =  5,
    WRITE_IN_FILE  =  6,
    READ_FROM_FILE =  7,
    UNKNOWN        =  8
} AkinatorMode_t;

typedef struct
{
    AkinatorMode_t num;
    const char* name;
    AkinatorErr_t (* func)(Node_t** root, int* count_img);
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

size_t GetUserAns(char** UserAns);
unsigned long int DGB2Hash(const char* str);
int Confirm(void);
AkinatorErr_t CreateNewNode(Node_t* cur_node);
FILE* TakeFileName(size_t* file_size, const char* mode);
char* SkipSpaces(char* ptr);

Node_t* PrintDefinitionBuffer(size_t i, Node_t*  node, const char** buffer);
AkinatorErr_t Definition(Node_t** root, int* count_img);
Node_t* FindObjectInTree(stack_s* definition_stack, Node_t* node, char* searching_object);
AkinatorErr_t CompareObjects(Node_t** root, int* count_img);
void ObjectsDefinitionDtor(stack_s* stk, char** obj);
AkinatorErr_t UpdateTree(Node_t** root, int* count_img);
AkinatorErr_t Guessing(Node_t** root, int* count_img);
AkinatorErr_t SayGoodby(Node_t** root, int* count_img);
AkinatorErr_t ShowTree(Node_t** root, int* count_img);
AkinatorErr_t WriteTreeInFile(Node_t** root, int* count_img);
char* ReadFile(void);
AkinatorErr_t ReadTreeFromFile(Node_t** root, int* count_img);
Node_t* ReadTreeNode(char** cur_pos);


const mode_s ModeStructArr[] =
{
    {DEFINITION,     "определить",              Definition      },
    {COMPARISON,     "сравнить",                CompareObjects  },
    {UPDATE,         "обновить",                UpdateTree      },
    {GUESSING,       "отгадывание",             Guessing        },
    {END,            "закончить",               SayGoodby       },
    {SHOW_TREE,      "просмотр",                ShowTree        },
    {WRITE_IN_FILE,  "запись",                  WriteTreeInFile },
    {READ_FROM_FILE, "чтение",                  ReadTreeFromFile},
    {UNKNOWN,        "UNKNOWN_CODE",            NULL            }
};

const size_t MODES_NUM = sizeof(ModeStructArr)/sizeof(ModeStructArr[0]);

#endif // AKINATOR_TYPES
