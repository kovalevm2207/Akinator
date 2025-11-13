#ifndef OUT_FUNCTIONS
#define OUT_FUNCTIONS
#include <cstddef>    // для size_t
#include <cstdio>     // для printf
#include <stdlib.h>
#include <unistd.h>

#include "AkinatorTypes.h"
#include "color_print.h"

const int SECOND = 1e6;

int HelloUser(void);
void PrintGetlineErr(void);
void PrintIncorrectAns(void);

#endif
