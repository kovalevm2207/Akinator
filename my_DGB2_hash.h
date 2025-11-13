#ifndef MY_DJB2_HASH
#define MY_DJB2_HASH
#include <cstddef>    // для size_t
#include <cstdio>     // для printf

#include "AkinatorTypes.h"
#include "color_print.h"

int QsortCompareFunc(const void* a, const void* b);
int FindEqualsHash(const hash_s* hashes);
unsigned long int DGB2Hash(const char* str);
hash_s* MakeHashes(hash_s* hashes);
AkinatorErr_t CheckModesNum(void);
void PrintModeStructArr(const hash_s* hashes);

#endif
