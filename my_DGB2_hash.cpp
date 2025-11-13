#include "my_DGB2_hash.h"

int QsortCompareFunc(const void* a, const void* b)
{
    const hash_s* hash_a = (const hash_s*) a;
    const hash_s* hash_b = (const hash_s*) b;

    if(hash_a->hash < hash_b->hash) return -1;
    if(hash_a->hash > hash_b->hash) return  1;

    return 0;
}


int FindEqualsHash(const hash_s* hashes)
{
    int status = 0;

    for (size_t i = 0; i < MODES_NUM; i++)
    {
        for(size_t j = i + 1; j < MODES_NUM; j++)
        {
            if (hashes[i].hash == hashes[j].hash)
            {
                printf(CHANGE_ON RED TEXT_COLOR "You have equal hashes for commands:\n"
                                                "mode = %10s ---> hash = %lu\n"
                                                "mode = %10s ---> hash = %lu\n" RESET,
                                                ModeStructArr[i].name, hashes[i].hash,
                                                ModeStructArr[j].name, hashes[j].hash);
                status |= 1;
            }
        }
    }

    return status;
}


unsigned long int DGB2Hash(const char* str)
{
    unsigned long int hash = 5381;  // магическое число над которым чувак бился пол жизни

    unsigned char c = 0;

    while ((c = (unsigned char) *str++) != '\0') {
        ON_DEBUG(printf("%c -> %d\n", *str, *str));
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}


hash_s* MakeHashes(hash_s* hashes)
{
    for (size_t i = 0; i < MODES_NUM; i++)
    {
        hashes[i].hash = DGB2Hash(ModeStructArr[i].name);
        hashes[i].num = ModeStructArr[i].num;
    }

    return hashes;
}


AkinatorErr_t CheckModesNum(void)
{
    AkinatorErr_t STATUS = AKINATOR_OK;
    for (long unsigned int i = 0; i < MODES_NUM; i++) {
        if (ModeStructArr[i].num != i) {
            printf( RED_COLOR "!!!ERROR!!! номера команд не соответствуют их позиции в массиве структур\n" RESET
                              "Ошибка произошла в команде %s\n"
                    RED_COLOR "Сейчас:      номер команды %d        позиция в массиве структур %lu\n" RESET
                  GREEN_COLOR "Должно быть: номер команды %d        позиция в массиве структур %d\n" RESET,
                              ModeStructArr[i].name, ModeStructArr[i].num, i, ModeStructArr[i].num, ModeStructArr[i].num);
            STATUS = MODES_NUM_ERR;
        }
    }

    return STATUS;
}


void PrintModeStructArr(const hash_s* hashes)
{
    printf("[ i] |  num | name\n");

    for (long unsigned int i = 0; i < MODES_NUM; i++)
    {
        printf("[%2lu] | %4d | %-30s\n", i, ModeStructArr[i].num, ModeStructArr[i].name);
    }
    for (long unsigned int i = 0; i < MODES_NUM; i++)
    {
        printf("[%2lu] | %4d | %20lu\n", i, hashes[i].num, hashes[i].hash);
    }
}
