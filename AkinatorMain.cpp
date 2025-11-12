#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>

#include "TreeMemStruct/Tree.h"
#include "color_print.h"

const int SECOND = 1e6;
const int START_LENGTH = 1 << 2;

typedef enum
{
    AKINATOR_OK = 1 << 0,
    MODES_NUM_ERR = 1 << 1
} AkinatorErr_t;

typedef enum
{
    DEFINITION = 0,
    COMPARISON = 1,
    UPDATE     = 2,
    GUESSING   = 3,
    END        = 4,
    ADD_NODE   = 5,
    YES        = 6,
    NO         = 7,
    UNKNOWN    = 8
} AkinatorMode_t;

typedef struct
{
    AkinatorMode_t num;
    const char* name;
    unsigned long int hash;
}mode_s;

mode_s ModeStructArr[] =
{
    {DEFINITION, "определение\n",   0},
    {COMPARISON, "сравнить\n",      0},
    {UPDATE,     "обновить\n",      0},
    {GUESSING,   "отгадывание\n",   0},
    {END,        "закончить\n",     0},
    {ADD_NODE,   "добавить узел\n", 0},
    {YES,        "да\n",            0},
    {NO,         "нет\n",           0},
    {UNKNOWN,    "UNKNOWN_CODE\n",  0}
};

size_t MODES_NUM = sizeof(ModeStructArr)/sizeof(ModeStructArr[0]);

AkinatorErr_t CheckModesNum(void);
void MakeHashes(void);
int FindEqualsHash(void);
void PrintModeStructArr(void);
unsigned long int DGB2Hash(const char* str);

int HelloUser(void);
char* GetUserAns(char* buffer);
AkinatorMode_t AnalyzeUserAns(const char* buffer);

#ifdef DEBUG
    #define ON_DEBUG(func) func
    #define DEBUG_OF(func)
#else
    #define ON_DEBUG(func)
    #define DEBUG_OF(func) func
#endif

int main()
{
    if (CheckModesNum() != AKINATOR_OK) {printf(RED_COLOR "MODES NUM ERROR\n" RESET); return 1;}
    MakeHashes();
    if (FindEqualsHash()) {printf(RED_COLOR "YOU HAVE EQUALS HASHES\n" RESET); return 1;}
    ON_DEBUG(PrintModeStructArr());

    if (HelloUser() <= 0) { printf(RED_COLOR "FATAL ERROR IN FUNCTION int HelloUser(void)\n" RESET); return 1;}

    AkinatorMode_t mode = UNKNOWN;
    do
    {
        char* UserAns = (char*) calloc(START_LENGTH, sizeof(char));
        if (UserAns == NULL) {printf(RED_COLOR "MEMORY ALLOCATION ERR\n" RESET); return 1;}

        UserAns = GetUserAns(UserAns);
        if (UserAns == NULL) {printf(RED_COLOR "ERR IN FUNC char* GetUserAns(char* buffer)\n"); free(UserAns); UserAns = NULL; return 1;}

        mode = AnalyzeUserAns(UserAns);
        if (mode == UNKNOWN)
        {
            printf(RED_COLOR "Не зли меня, формулируй свои запросы правильно, вселенная очень капризна и не отвечает на что попало\n\n\t\t" RESET);
            continue;
        }

        free(UserAns);
        UserAns = NULL;
    } while (mode != END);

    return 0;
}


void PrintModeStructArr(void)
{
    printf("[ i] |  num | name                      hash               |\n");

    for (size_t i = 0; i < MODES_NUM; i++)
    {
        printf("[%2zu] | %4d | %-30s  %20zu |\n", i, ModeStructArr[i].num, ModeStructArr[i].name, ModeStructArr[i].hash);
    }
}


int FindEqualsHash(void)
{
    int status = 0;

    for (size_t i = 0; i < MODES_NUM; i++)
    {
        for(size_t j = i + 1; j < MODES_NUM; j++)
        {
            if (ModeStructArr[i].hash == ModeStructArr[j].hash)
            {
                printf(CHANGE_ON RED TEXT_COLOR "You have equal hashes for commands:\n"
                                                "%10s ---> hash = %zu\n"
                                                "%10s ---> hash = %zu\n" RESET,
                                                ModeStructArr[i].name, ModeStructArr[i].hash,
                                                ModeStructArr[j].name, ModeStructArr[j].hash);
                status |= 1;
            }
        }
    }

    return status;
}


unsigned long int DGB2Hash(const char* str)
{
    unsigned long int hash = 5381;

    unsigned char c;

    while ((c = (unsigned char) *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}


void MakeHashes(void)
{
    for (size_t i = 0; i < MODES_NUM; i++)
    {
        ModeStructArr[i].hash = DGB2Hash(ModeStructArr[i].name);
    }
}


AkinatorErr_t CheckModesNum(void)
{
    AkinatorErr_t STATUS = AKINATOR_OK;
    for (size_t i = 0; i < MODES_NUM; i++) {
        if (ModeStructArr[i].num != i) {
            printf( RED_COLOR "!!!ERROR!!! номера команд не соответствуют их позиции в массиве структур\n" RESET
                              "Ошибка произошла в команде %s\n"
                    RED_COLOR "Сейчас:      номер команды %d        позиция в массиве структур %zu\n" RESET
                  GREEN_COLOR "Должно быть: номер команды %d        позиция в массиве структур %d\n" RESET,
                              ModeStructArr[i].name, ModeStructArr[i].num, i, ModeStructArr[i].num, ModeStructArr[i].num);
            STATUS = MODES_NUM_ERR;
        }
    }

    return STATUS;
}


AkinatorMode_t AnalyzeUserAns(const char* buffer)
{
    AkinatorMode_t mode = UNKNOWN;

    size_t ans_hash = DGB2Hash(buffer);
    ON_DEBUG(printf("\tanswer's hash = %zu\n", ans_hash););
    if (ans_hash == ModeStructArr[END].hash) return END;

    return mode;
}


char* GetUserAns(char* buffer)
{
    long unsigned int CUR_LENGTH = START_LENGTH;
    long unsigned int LAST_LENGTH = 0;

    while (1)
    {
    if (fgets(buffer + LAST_LENGTH, (int) (CUR_LENGTH - LAST_LENGTH), stdin) == NULL)
    {
        printf("3");
        printf(RED_COLOR "FGETS USER ANSWER ERR\n" RESET); return NULL;
    }

    ON_DEBUG(printf("Read: '");
    for(long unsigned int i = 0; i < CUR_LENGTH; i++) {
        switch(buffer[i])
        {
            case '\0': printf("\\0"); break;
            case '\n': printf("\\n"); break;
            default: printf("%c", buffer[i]); break;
        }
    }
    printf("'\n");)

    char* slash_n_pos = strchr(buffer, '\n');

    ON_DEBUG(printf("strchr result: %p\n", slash_n_pos);)

    if (slash_n_pos == NULL)
    {
        char* Newbuffer = (char*) realloc(buffer, CUR_LENGTH * 2);
        if (Newbuffer == NULL) {printf(RED_COLOR "MEMORY ALLOCATION ERR\n" RESET); return NULL;}

        buffer = Newbuffer;
        memset(buffer + CUR_LENGTH, '\0', sizeof(char) * CUR_LENGTH);
        LAST_LENGTH = CUR_LENGTH - 1;
        CUR_LENGTH *= 2;

        continue;
    }
    else break;
    }

    ON_DEBUG(fputs(buffer, stdout);)
    return buffer;
}

int HelloUser(void)
{

// to do: Вообще было бы шикарно переделать так, чтобы выводилось посимвольно,
// что-то вроде бегущего текста, и чтобы были учтены задержки при знаках препинания.
// например между буквами одна задержка
//          между словами чуть побольше
//          если запятая, то пауза, и т.д.
//  Это позволит сделать интерфейс программы более приятным для взгляда , так как строчки не будут появляться сразу, внезапно

    #define PRINT return_value+= printf

    #ifndef DEBUG
        #ifdef _WIN32
            system("cls");
        #elif defined(__linux__)
            system("clear");
        #else
            printf("О путник, на тебе лежит злое проклятие лорда маковода, прости ,но я не могу тебе ничем помочь, уходи...\n");
        #endif
    #endif

    int return_value = printf("Добро пожаловать , о путник, страждущий знаний...\n");
    DEBUG_OF(usleep(2 * SECOND);)
    PRINT("Ты попал ко мне - всезнающему(ну почти), великому(не ну еще не на столько)\n\n");
    DEBUG_OF(usleep(2.5 * SECOND);)
    PRINT(RED_COLOR "                            АКИНАТОРУ                                     \n\n" RESET);
    DEBUG_OF(usleep(4 * SECOND);)
    PRINT("Со мной ты можешь сделать то, что обычному человеку не подвластно, а именно:\n\n");
    DEBUG_OF(usleep(1.5 * SECOND);)
    PRINT("-- " CYAN_COLOR "дать определение" RESET ", любому предмету, существу или явлению (которое я знаю) "
          "------------ " GREEN_COLOR  "[определение]\n" RESET);
    PRINT("-- " CYAN_COLOR  "найти общее" RESET " между двумя предметами и то, чем они отличаются "
          "------------------------- " GREEN_COLOR "[сравнить]\n" RESET);
    PRINT("-- " CYAN_COLOR "уничтожить все" RESET ", что я знаю и начать с начала вместе с тобой (не надо пожалуйста) "
          "---- " GREEN_COLOR "[обновить]\n\n" RESET);
    DEBUG_OF(usleep(8 * SECOND);)
    PRINT("Но самое главное, то ,благодаря чему я стал настолько известным и великим --");
    DEBUG_OF(usleep(2 * SECOND);)
    PRINT(" я могу " CYAN_COLOR "прочитать твои мысли" RESET ",\n"
          "узнать, что же творится в твоей голове "
          "----------- " GREEN_COLOR "[отгадывание]\n\n" RESET);
    DEBUG_OF(usleep(4 * SECOND);)
    PRINT(PURPLE_COLOR "Готов ли ты окунуться со мной в удивительный мир знаний и ясновидения???\n\n" RESET);
    DEBUG_OF(usleep(5 * SECOND);)
    PRINT("Если да , то скорее выбирай чем мы будем заниматься " GREEN_COLOR "[определение] [сравнить] "
                                                                             "[обновить] [отгадывание]\n\n" RESET);
    DEBUG_OF(usleep(3 * SECOND);)
    PRINT("Если же ты еще не готов ко моей силе, то можешь пока проститься со мной "
          GREEN_COLOR "[закончить]\n" RESET);
    DEBUG_OF(usleep(3 * SECOND);)
    PRINT("(Но помни, я всегда рядом и ты можешь вернуться к моим необъятным познаниям... )\n\n");
    DEBUG_OF(usleep(3 * SECOND);)
    PRINT("Скорее дай мне знать, чего ты хочешь:\n\n""\t\t");

    return return_value;
}
