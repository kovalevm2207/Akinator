#include "AkinatorTypes.h"

size_t GetUserAns(char** UserAns)
{
    assert(UserAns != NULL);

    ssize_t len = 0;
    size_t size = 0;
    if ((len = getline(UserAns, &size, stdin)) <= 0) {printf(RED_COLOR "getline USER ANSWER ERR\n" RESET); return 1;}
    (*UserAns)[(size_t) len - 1] = '\0';
    return (size_t) len;
}


unsigned long int DGB2Hash(const char* str)
{
    assert(str != NULL);

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
    assert(root != NULL);
    assert(count_img != NULL);

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
    assert(root != NULL);
    assert(count_img != NULL);

    (void) count_img;
    (void) root;

    printf("\nПрощай путник, вселенная подсказывает , что мы еще увидимся . . . :)\n");
    return AKINATOR_OK;
}


AkinatorErr_t UpdateTree(Node_t** root, int* count_img)
{
    assert(root != NULL);
    assert(count_img != NULL);

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
    assert(cur_node != NULL);

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
    assert(root != NULL);
    assert(count_img != NULL);

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
                cur_node = *root;
            }
        }
    }

    return AKINATOR_OK;
}


FILE* TakeFileName(size_t* file_size, const char* mode)
{
    assert(file_size != NULL);
    assert(mode != NULL);

    char* UserAns = NULL;
    GetUserAns(&UserAns);
    if (strcmp(mode, "r") == 0) *file_size = find_file_size(UserAns);

    FILE* file = fopen(UserAns, mode);

    free(UserAns);
    UserAns = NULL;
    return file;
}


AkinatorErr_t WriteTreeInFile(Node_t** root, int* count_img)
{
    assert(root != NULL);
    assert(count_img != NULL);

    (void) count_img;

    PrintWhereWrite();
    size_t file_size = 0;
    FILE* file = TakeFileName(&file_size, "w");
    if(file == NULL) return OPEN_FILE_ERR;

    PrintTreeNode(file, *root, "l");
    fclose(file);
    file = NULL;

    return AKINATOR_OK;
}


char* ReadFile(void)
{
    PrintReadFromQuestion();

    size_t file_size = 0;
    FILE* file = TakeFileName(&file_size, "r");
    if(file == NULL) return NULL;

    char* buffer = (char*) calloc(file_size + 1,sizeof(char));
    if (buffer == NULL)
    {
        fclose(file);
        file = NULL;
        return NULL;
    }

    size_t ret = fread(buffer, sizeof(char), file_size, file);
    if (ret != file_size)
    {
        fclose(file);
        free(buffer);
        file = NULL;
        buffer = NULL;
        return NULL;
    }
    fclose(file);
    file = NULL;

    return buffer;
}


char* SkipSpaces(char* ptr)
{
    assert(ptr != NULL);

    while(*ptr && isspace((unsigned char) *ptr))
    {
        switch(*ptr)
        {
            case '\n':
                ON_DEBUG(printf("enter\n"));
                break;
            case ' ':
                ON_DEBUG(printf("space\n"));
                break;
            default:
                ON_DEBUG(printf("%c", *ptr));
                break;
        }

        ptr++;
    }
    return ptr;
}


AkinatorErr_t ReadTreeFromFile(Node_t** root, int* count_img)
{
    assert(root != NULL);
    assert(count_img != NULL);

    (void) count_img;
    UpdateTree(root, count_img);

    char* buffer = ReadFile();
    if (buffer == NULL) return READ_FILE_ERR;
    char* cur_pos = SkipSpaces(buffer);

    *root = ReadTreeNode(&cur_pos);

    free(buffer);
    buffer = NULL;

    return AKINATOR_OK;
}


Node_t* ReadTreeNode(char** cur_pos)
{
    assert(cur_pos != NULL);

    Node_t* node = NULL;

    if(**cur_pos == '(')
    {
        *cur_pos = SkipSpaces(*cur_pos + 1);
        int start_pos = 0, end_pos = 0;
        sscanf(*cur_pos, "\"%n%*[^\"]%n\"", &start_pos, &end_pos);

        int len = end_pos - start_pos;
        node = TreeNodeCtor(strndup(*cur_pos + start_pos, (size_t) len), NULL, NULL);

        *cur_pos = SkipSpaces(*cur_pos + end_pos + 1);
        Node_t* left_son = ReadTreeNode(cur_pos);
        TreeInsertLeft (node, left_son);

        Node_t* right_son = ReadTreeNode(cur_pos);
        TreeInsertRight(node, right_son);

        *cur_pos = SkipSpaces(*cur_pos);
        if (**cur_pos == ')')
        {
            *cur_pos = SkipSpaces(*cur_pos + 1);
            ON_DEBUG(printf(RED_COLOR ")\n" RESET));
            return node;
        }
        else {free(node); node = NULL; return NULL;}
    }
    else if (strncmp(*cur_pos, "nil", sizeof("nil") - 1) == 0)
    {
        *cur_pos += sizeof("nil") - 1;
        *cur_pos = SkipSpaces(*cur_pos);
        return NULL;
    }
    else return NULL;
}


AkinatorErr_t Definition(Node_t** root, int* count_img)
{
    assert(root != NULL);
    assert(count_img != NULL);
    (void) count_img;

    size_t start_capacity = 64;
    stack_s definition_stack = {};
    char* UserAns = NULL;

    do
    {
        PrintWhichObject();
        StackCtor(&definition_stack, start_capacity);

        GetUserAns(&UserAns);

        if (FindObjectInTree(&definition_stack, *root, UserAns))
        {
            PrintObjectDefinition(definition_stack, *root);
        }
        else
        {
            PrintNotFoundObject(UserAns);
        }

        free(UserAns);
        UserAns = NULL;
        StackDtor(&definition_stack);
        PrintContinueQuestion();
    }
    while(Confirm());

    return AKINATOR_OK;
}


Node_t* FindObjectInTree(stack_s* definition_stack, Node_t* node, char* searching_object)
{
    assert(definition_stack != NULL);
    assert(searching_object != NULL);
    assert(node != NULL);

    if(strcmp(searching_object, node->data) == 0)
    {
        StackPush(definition_stack, node->data);
        return node;
    }

    Node_t* result_node = NULL;
    bool left_tree = true;
    bool right_tree = false;
    bool left_or_right_tree_param = left_tree;


    if(node->left)
        result_node = FindObjectInTree(definition_stack, node->left, searching_object);
    if(result_node == NULL && node->right)
    {
        result_node = FindObjectInTree(definition_stack, node->right, searching_object);
        left_or_right_tree_param = right_tree;
    }
    if(result_node != NULL)
    {
        if (left_or_right_tree_param == left_tree)
            StackPush(definition_stack, "l");
        else
        {
            StackPush(definition_stack, "r");
        }
    }

    return result_node;
}
