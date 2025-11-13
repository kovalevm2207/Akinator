#include "AkinatorTypes.h"

AkinatorErr_t ShowTree(Node_t* root, int* count_img)
{
    if (TreeDump(root, *count_img) == TREE_OK)
    {
        (*count_img)++;
        ShowTreePrint();
        return AKINATOR_OK;
    }
    else
    {
        ErrShowTreePrint();
        return SHOW_TREE_ERR;
    }
}


AkinatorErr_t SayGoodby(Node_t* root, int* count_img)
{
    (void) count_img;
    (void) root;
    
    printf("\nПрощай путник, вселенная подсказывает , что мы еще увидимся . . . :)\n");
    return AKINATOR_OK;
}


AkinatorErr_t UpdateTree(Node_t* root, int* count_img)
{
    (void) count_img;
    DeleteTreeNode(root);

    root = TreeNodeCtor("ROOT", NULL, NULL);
    if (root == NULL) return UPDATE_ERR;
    else return AKINATOR_OK;
}
