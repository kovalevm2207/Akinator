#ifndef OUT_FUNCTIONS
#define OUT_FUNCTIONS

#include "AkinatorTypes.h"

const int SECOND = 1e6;


#define PRINT(text) return_value+= printf("%s", text)
#ifndef DEBUG
    #define SPEAK(text) system("espeak -v ru -s 90 -a 120 -p 50 \"" text "\"")
#else
    #define SPEAK(text)
#endif

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
void PrintWhereWrite(void);
void PrintReadFromQuestion(void);
void PrintWhichObject(void);
void PrintNotFoundObject(char* searching_object);
void PrintObjectDefinition(stack_s definition_stack, Node_t* root);
void PrintDiff(Node_t* root, stack_s first_obj_definition, stack_s second_obj_definition);
void PrintFirstObjQuestion(void);
void PrintSecondObjQuestion(void);
void PrintTreeEmpty(void);

#endif
