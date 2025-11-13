#include "Akinator.h"


int main()
{
    hash_s hashes[MODES_NUM] = {};
    // проверять порядок номеров по-моему нет смысла, так как мы все равно сортируем потом
    MakeHashes(hashes);
    if (FindEqualsHash(hashes)) {printf(RED_COLOR "YOU HAVE EQUALS HASHES\n" RESET); return 1;}
    ON_DEBUG(printf("Массив с хешами до сортировки:\n"));
    ON_DEBUG(PrintModeStructArr(hashes));
    qsort(hashes, MODES_NUM, sizeof(hashes[0]), QsortCompareFunc);
    ON_DEBUG(printf("\n\n"));
    ON_DEBUG(printf("Массив с хешами после сортировки:\n"));
    ON_DEBUG(PrintModeStructArr(hashes));


    if (HelloUser() <= 0) { printf(RED_COLOR "FATAL ERROR IN FUNCTION int HelloUser(void)\n" RESET); return 1;}


    AkinatorMode_t mode = UNKNOWN;
    do
    {
        char* UserAns = NULL;

        #ifdef __linux__
            size_t size = 0;
            ssize_t len = 0;
            if ((len = getline(&UserAns, &size, stdin)) <= 0) {printf(RED_COLOR "getline USER ANSWER ERR\n" RESET); return 1;}
            UserAns[len - 1] = '\0';
        #elif defined(_WIN32)
            // Моя реализация getline() ... зачем ... ааа для windows повезло повезло ))))
            UserAns = (char*) calloc(START_LENGTH, sizeof(char));
            if (UserAns == NULL) {printf(RED_COLOR "MEMORY ALLOCATION ERR\n" RESET); return 1;}
            UserAns = GetUserAns(UserAns);
            // TXSpeak("a b c d e f g h i j k l m o n");
        #endif

        if (UserAns == NULL) {printf(RED_COLOR "ERR IN FUNC char* GetUserAns(char* buffer)\n"); free(UserAns); UserAns = NULL; return 1;}


        mode = AnalyzeUserAns(UserAns, hashes);
        if (mode == UNKNOWN)
        {
            printf(RED_COLOR "Не зли меня, формулируй свои запросы правильно, вселенная очень капризна и не отвечает на что попало\n\n\t\t" RESET);
            free(UserAns);
            continue;
        }


        free(UserAns);
        UserAns = NULL;
    } while (mode != END);

    return 0;
}


AkinatorMode_t AnalyzeUserAns(const char* buffer, hash_s* hashes)
{
    AkinatorMode_t mode = UNKNOWN;

    size_t ans_hash = DGB2Hash(buffer);
    ON_DEBUG(printf("\tanswer's hash = %lu\n", ans_hash););

    hash_s* cur_mode = (hash_s*) bsearch(&ans_hash, hashes, MODES_NUM, sizeof(hashes[0]), BsearchCompareFunc);

    if(cur_mode) mode = cur_mode->num;

    ON_DEBUG(printf("\tfounded mode = %d\n\t", mode);)

    return mode;
}


// передаю unsigned long int*, а в массиве лежат mode_s
int BsearchCompareFunc(const void* searching_elem, const void* cur_elem)
{
    const unsigned long int* searching_elem_ = (const unsigned long int*) searching_elem;
    const hash_s* cur_elem_ = (const hash_s*) cur_elem;

    if (*searching_elem_ < cur_elem_->hash) return -1;
    if (*searching_elem_ > cur_elem_->hash) return  1;

    return 0;
}


char* GetUserAns(char* buffer)
{
    long unsigned int CUR_LENGTH = START_LENGTH;
    long unsigned int LAST_LENGTH = 0;
    char* slash_n_pos = 0;

    while (1)
    {
        if (fgets(buffer + LAST_LENGTH, (int) (CUR_LENGTH - LAST_LENGTH), stdin) == NULL)
        {
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

        slash_n_pos = strchr(buffer, '\n');

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

    *slash_n_pos = '\0';
    ON_DEBUG(fputs(buffer, stdout);)
    return buffer;
}
