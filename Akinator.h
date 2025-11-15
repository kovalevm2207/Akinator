#ifndef AKINATOR
#define AKINATOR

#include "AkinatorTypes.h"
#include "my_DGB2_hash.h"
#include "OutFunctions.h"

const int START_LENGTH = 1 << 2;

/*unused*/ char* MyGetline(char* buffer);

int WorkWithHashes(hash_s* hashes);
AkinatorMode_t AnalyzeUserAns(const char* buffer, hash_s* hashes);
int BsearchCompareFunc(const void* searching_elem, const void* cur_elem);
AkinatorErr_t DoMode(AkinatorMode_t mode, Node_t** root, int* count_img);

#endif // AKINATOR
