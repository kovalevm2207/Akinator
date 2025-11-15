#ifndef OUT_FUNCTIONS
#define OUT_FUNCTIONS

#include "AkinatorTypes.h"

const int SECOND = 1e6;


#define PRINT(text) return_value+= printf("%s", text)
#define SPEAK(text) system("espeak -v ru -s 90 -a 120 -p 50 \"" text "\"")

int HelloUser(void);
void PrintGetlineErr(void);
void PrintIncorrectAns(void);
void ShowTreePrint(void);
void ErrShowTreePrint(void);
void PrintContinueQuestion(void);
void PrintSuccess(void);
void PrintQuestionWho(void);
void PrintComparisonQuestion(const char* left, const char* right);
void PrintIRemember(void);
void PrintSecondStartTree(void);
void PrintStartTree(void);
void PrintDeleteAns(void);
void PrintContinue(void);

#endif
