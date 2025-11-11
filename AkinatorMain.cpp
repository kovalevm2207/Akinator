#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "TreeMemStruct/Tree.h"
#include "color_print.h"

const int SECOND = 1e6;
const int START_LENGTH = 1 << 2;

typedef enum
{
    AKINATOR_OK = 1 << 0
} AkinatorErr_t;

typedef enum
{
    DEFINITION = 1,
    COMPARISON = 2,
    UPDATE     = 3,
    GUESSING   = 4,
    END        = 5
} AkinatorMode_t;

char* GetUserAns(char* buffer);
int HelloUser(void);

#ifdef DEBUG
    #define ON_DEBUG(func) func
#else
    #define ON_DEBUG(func)
#endif

int main()
{
    if (HelloUser() <= 0) { printf(RED_COLOR "FATAL ERROR IN FUNCTION int HelloUser(void)\n" RESET); return 0;}

    AkinatorMode_t AM = END;
    do
    {
        char* UserAns = (char*) calloc(START_LENGTH, sizeof(char));
        if (UserAns == NULL) {printf(RED_COLOR "MEMORY ALLOCATION ERR\n" RESET); return 1;}

        UserAns = GetUserAns(UserAns);
        if (UserAns == NULL) {printf(RED_COLOR "ERR IN FUNC char* GetUserAns(char* buffer)\n"); free(UserAns); UserAns = NULL; return 1;}

        

        free(UserAns);
        UserAns = NULL;
    } while (AM != END);

    return 0;
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

    #ifdef _WIN32
        system("cls");
    #elif defined(__linux__)
        system("clear");
    #else
        printf("О путник, на тебе лежит злое проклятие лорда маковода, прости ,но я не могу тебе ничем помочь, уходи...\n");
    #endif

    int return_value = printf("Добро пожаловать , о путник, страждущий знаний...\n");
    usleep(2 * SECOND);
    PRINT("Ты попал ко мне - всезнающему(ну почти), великому(не ну еще не на столько)\n\n");
    usleep(2.5 * SECOND);
    PRINT(RED_COLOR "                            АКИНАТОРУ                                     \n\n" RESET);
    usleep(4 * SECOND);
    PRINT("Со мной ты можешь сделать то, что обычному человеку не подвластно, а именно:\n\n");
    usleep(1.5 * SECOND);
    PRINT("-- " CYAN_COLOR "дать определение" RESET ", любому предмету, существу или явлению (которое я знаю) "
          "------------ " GREEN_COLOR  "[определение]\n" RESET);
    PRINT("-- " CYAN_COLOR  "найти общее" RESET " между двумя предметами и то, чем они отличаются "
          "------------------------- " GREEN_COLOR "[сравнить]\n" RESET);
    PRINT("-- " CYAN_COLOR "уничтожить все" RESET ", что я знаю и начать с начала вместе с тобой (не надо пожалуйста) "
          "---- " GREEN_COLOR "[обновить]\n\n" RESET);
    usleep(8 * SECOND);
    PRINT("Но самое главное, то ,благодаря чему я стал настолько известным и великим --");
    usleep(2 * SECOND);
    PRINT(" я могу " CYAN_COLOR "прочитать твои мысли" RESET ",\n"
          "узнать, что же творится в твоей голове "
          "----------- " GREEN_COLOR "[отгадывание]\n\n" RESET);
    usleep(4 * SECOND);
    PRINT(PURPLE_COLOR "Готов ли ты окунуться со мной в удивительный мир знаний и ясновидения???\n\n" RESET);
    usleep(5 * SECOND);
    PRINT("Если да , то скорее выбирай чем мы будем заниматься " GREEN_COLOR "[определение] [сравнить] "
                                                                             "[обновить] [отгадывание]\n\n" RESET);
    usleep(3 * SECOND);
    PRINT("Если же ты еще не готов ко моей силе, то можешь пока проститься со мной "
          GREEN_COLOR "[закончить]\n" RESET);
    usleep(3 * SECOND);
    PRINT("(Но помни, я всегда рядом и ты можешь вернуться к моим необъятным познаниям... )\n\n");
    usleep(3 * SECOND);
    PRINT("Скорее дай мне знать, чего ты хочешь:\n""\t\t");

    return return_value;
}
