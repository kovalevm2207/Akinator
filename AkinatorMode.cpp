#include "AkinatorTypes.h"

size_t GetUserAns(char** UserAns)
{
    size_t len = 0;
    #ifdef __linux__
        ssize_t len_ = 0;
        size_t size = 0;
        if ((len_ = getline(UserAns, &size, stdin)) <= 0) {printf(RED_COLOR "getline USER ANSWER ERR\n" RESET); return 1;}
        len = (size_t) len_;
        (*UserAns)[len - 1] = '\0';
    #elif defined(_WIN32)
        // Моя реализация getline() ... зачем ... ааа для windows повезло повезло)))), хотя вроде getline должен быть в TXLib.h?
        UserAns = (char*) calloc(START_LENGTH, sizeof(char));
        if (UserAns == NULL) {printf(RED_COLOR "MEMORY ALLOCATION ERR\n" RESET); return 1;}
        len = MyGetline(UserAns);
        // TXSpeak("a b c d e f g h i j k l m o n"); // ну может будет когда-то
    #endif

    return len;
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


AkinatorErr_t ShowTree(Node_t** root, int* count_img)
{
    TreeErr_t status = TreeDump(*root, *count_img);
    if (status == TREE_OK || status == NULL_NODE)
    {
        ShowTreePrint();
        getchar();
        system("google-chrome dump.html > /dev/null 2>&1 &");
        (*count_img)++;
        return AKINATOR_OK;
    }
    else
    {
        ErrShowTreePrint();
        return SHOW_TREE_ERR;
    }
}


AkinatorErr_t SayGoodby(Node_t** root, int* count_img)
{
    (void) count_img;
    (void) root;

    printf("\nПрощай путник, вселенная подсказывает , что мы еще увидимся . . . :)\n");
    return AKINATOR_OK;
}


AkinatorErr_t UpdateTree(Node_t** root, int* count_img)
{
    (void) count_img;
    DeleteTreeNode(root);

    PrintDeleteAns();
    if (Confirm()) StartHTMLfile();

    return AKINATOR_OK;
}


int Confirm(void)
{
    char* UserAns = NULL;
    unsigned long int ans_hash = 0;
    const unsigned long int yes = DGB2Hash("да"), no = DGB2Hash("нет");

    while (true)
    {
        GetUserAns(&UserAns);
        ans_hash = DGB2Hash(UserAns);
        free(UserAns);
        UserAns = NULL;

        if (ans_hash == yes) return true;
        else if(ans_hash == no) return false;
        else PrintIncorrectAns();
    }
}


AkinatorErr_t CreateNewNode(Node_t* cur_node)
{
    char* UserAns = NULL;

    PrintQuestionWho();
    GetUserAns(&UserAns);
    PrintComparisonQuestion(UserAns, cur_node->data);
    Node_t* left_object = TreeNodeCtor(UserAns, NULL, NULL);

    GetUserAns(&UserAns);
    Node_t* right_object = TreeNodeCtor(cur_node->data, NULL, NULL);

    cur_node->data = UserAns;

    TreeInsertLeft (cur_node,  left_object);
    TreeInsertRight(cur_node, right_object);

    PrintIRemember();

    return AKINATOR_OK;
}


AkinatorErr_t Guessing(Node_t** root, int* count_img)
{
    (void) count_img;
    char* UserAns = NULL;

    if (*root == NULL)
    {
        PrintStartTree();
        GetUserAns(&UserAns);
        *root = TreeNodeCtor(UserAns, NULL, NULL);

        PrintIRemember();
        PrintContinueQuestion();
        if (!Confirm()) return AKINATOR_OK;
    }
    Node_t* cur_node = *root;
    while (cur_node)
    {
        printf("%s?\n\n\t\t", cur_node->data);
        if (Confirm())
        {
            if (cur_node->left) cur_node =  cur_node->left;
            else
            {
                PrintSuccess();
                PrintContinueQuestion();
                if (!Confirm()) return AKINATOR_OK;
                cur_node = *root;
            }
        }
        else
        {
            if (cur_node->right) cur_node = cur_node->right;
            else
            {
                CreateNewNode(cur_node);
                PrintContinueQuestion();
                if (!Confirm()) return AKINATOR_OK;
            }
        }
    }

    return AKINATOR_OK;
}


AkinatorErr_t WriteTreeInFile(Node_t** root, int* count_img)
{
    (void) count_img;

    PrintWhereWrite();
    char* UserAns = NULL;
    GetUserAns(&UserAns);
    FILE* file = fopen(strcat(UserAns,".txt"), "w");
    free(UserAns);
    UserAns = NULL;
    if(file == NULL) return OPEN_FILE_ERR;

    PrintTreeNode(file, *root, "l");
    fclose(file);
    file = NULL;

    return AKINATOR_OK;
}
