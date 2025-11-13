#ifndef AKINATOR_TYPES
#define AKINATOR_TYPES

typedef enum
{
    AKINATOR_OK = 1 << 0,
    MODES_NUM_ERR = 1 << 1
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
    YES            =  8,
    NO             =  9,
    UNKNOWN        = 10
} AkinatorMode_t;

typedef struct
{
    AkinatorMode_t num;
    const char* name;
}mode_s;

typedef struct
{
    unsigned long hash;
    AkinatorMode_t num;
} hash_s;

const mode_s ModeStructArr[] =
{
    {DEFINITION,     "определение"           },
    {COMPARISON,     "сравнить"              },
    {UPDATE,         "обновить"              },
    {GUESSING,       "отгадывание"           },
    {END,            "закончить"             },
    {ADD_NODE,       "добавить узел"         },
    {SHOW_TREE,      "показать древо знаний" },
    {WRIGHT_IN_FILE, "записать знания в файл"},
    {YES,            "да"                    },
    {NO,             "нет"                   },
    {UNKNOWN,        "UNKNOWN_CODE"          }
};

const size_t MODES_NUM = sizeof(ModeStructArr)/sizeof(ModeStructArr[0]);

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

#endif // AKINATOR_TYPES
