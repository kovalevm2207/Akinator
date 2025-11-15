#ifndef MY_DJB2_HASH
#define MY_DJB2_HASH

#include "AkinatorTypes.h"

int QsortCompareFunc(const void* a, const void* b);
int FindEqualsHash(const hash_s* hashes);
hash_s* MakeHashes(hash_s* hashes);
AkinatorErr_t CheckModesNum(void);
void PrintModeStructArr(const hash_s* hashes);

#endif
