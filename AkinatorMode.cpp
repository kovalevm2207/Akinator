#include "AkinatorTypes.h"

int GetUserAns(char** UserAns)
{
    #ifdef __linux__
        size_t size = 0;
        ssize_t len = 0;
        if ((len = getline(UserAns, &size, stdin)) <= 0) {printf(RED_COLOR "getline USER ANSWER ERR\n" RESET); return 1;}
        (*UserAns)[len - 1] = '\0';
    #elif defined(_WIN32)
        // Моя реализация getline() ... зачем ... ааа для windows повезло повезло)))), хотя вроде getline должен быть в TXLib.h?
        UserAns = (char*) calloc(START_LENGTH, sizeof(char));
        if (UserAns == NULL) {printf(RED_COLOR "MEMORY ALLOCATION ERR\n" RESET); return 1;}
        UserAns = MyGetline(UserAns);
        // TXSpeak("a b c d e f g h i j k l m o n"); // ну может будет когда-то
    #endif

    return 0;
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
    unsigned long int ans_hash = 0;
    char* UserAns = NULL;
    const unsigned long int yes = DGB2Hash("да"), no = DGB2Hash("нет");
    DeleteTreeNode(root);

    PrintDeleteAns();

    int correct_ans = 0;
    while (!correct_ans)
    {
        GetUserAns(&UserAns);
        ans_hash = DGB2Hash(UserAns);
        free(UserAns);
        UserAns = NULL;

        if (ans_hash == yes)
        {
            StartHTMLfile();
            correct_ans = 1;
        }
        else if(ans_hash == no)
        {
            correct_ans = 1;
        }
        else PrintIncorrectAns();
    }

    return AKINATOR_OK;
}


AkinatorErr_t Guessing(Node_t** root, int* count_img)
{
    (void) count_img;

    char* UserAns = NULL;
    unsigned long int ans_hash = 0;
    const unsigned long int yes = DGB2Hash("да"), no = DGB2Hash("нет");

    if (*root == NULL)
    {
        PrintStartTree();
        GetUserAns(&UserAns);
        Node_t* left = TreeNodeCtor(UserAns, NULL, NULL);
        free(UserAns);
        UserAns = NULL;

        PrintSecondStartTree();
        GetUserAns(&UserAns);
        Node_t* right = TreeNodeCtor(UserAns, NULL, NULL);
        free(UserAns);
        UserAns = NULL;

        PrintComparisonQuestion(left->data, right->data);
        GetUserAns(&UserAns);
        *root = TreeNodeCtor(UserAns, left, right);
        free(UserAns);
        UserAns = NULL;

        PrintIRemember();

        PrintContinueQuestion();
        int correct_ans = 0;
        while (!correct_ans)
        {
            GetUserAns(&UserAns);
            ans_hash = DGB2Hash(UserAns);
            free(UserAns);
            UserAns = NULL;

            if (ans_hash == yes)
            {
                correct_ans = 1;
            }
            else if(ans_hash == no)
            {
                correct_ans = 1;
                return AKINATOR_OK;
            }
            else PrintIncorrectAns();
        }
    }

    Node_t* cur_node = *root;

    while (cur_node)
    {
        printf("%s?\n\n\t\t", cur_node->data);

        if(GetUserAns(&UserAns)) return GET_ANS_ERR;
        ans_hash = DGB2Hash(UserAns);
        free(UserAns);
        UserAns = NULL;

        if (ans_hash == yes)
        {
            if (cur_node->left) cur_node =  cur_node->left;
            else
            {
                PrintSuccess();
                PrintContinueQuestion();
                int correct_ans = 0;
                while (!correct_ans)
                {
                    GetUserAns(&UserAns);
                    ans_hash = DGB2Hash(UserAns);
                    free(UserAns);
                    UserAns = NULL;

                    if (ans_hash == yes)
                    {
                        correct_ans = 1;
                        cur_node = *root;
                    }
                    else if(ans_hash == no)
                    {
                        correct_ans = 1;
                        return AKINATOR_OK;
                    }
                    else PrintIncorrectAns();
                }
            }
        }
        else if(ans_hash ==  no)
        {
            if (cur_node->right) cur_node = cur_node->right;
            else
            {
                PrintQuestionWho();
                GetUserAns(&UserAns);
                Node_t* left_object = TreeNodeCtor(UserAns, NULL, NULL);

                PrintComparisonQuestion(UserAns, cur_node->data);

                free(UserAns);
                UserAns = NULL;

                GetUserAns(&UserAns);

                Node_t* right_object = TreeNodeCtor(cur_node->data, NULL, NULL);

                free(cur_node->data);
                cur_node->data = NULL;
                cur_node->data  = strdup(UserAns);
                free(UserAns);
                UserAns = NULL;

                TreeInsertLeft (cur_node,  left_object);
                TreeInsertRight(cur_node, right_object);

                PrintIRemember();

                PrintContinueQuestion();
                int correct_ans = 0;
                while (!correct_ans)
                {
                    GetUserAns(&UserAns);
                    ans_hash = DGB2Hash(UserAns);
                    free(UserAns);
                    UserAns = NULL;

                    if (ans_hash == yes)
                    {
                        correct_ans = 1;
                        cur_node = *root;
                    }
                    else if(ans_hash == no)
                    {
                        correct_ans = 1;
                        return AKINATOR_OK;
                    }
                    else PrintIncorrectAns();
                }
            }
        }
        else
        {
            PrintIncorrectAns();
        }
    }

    return AKINATOR_OK;
}
