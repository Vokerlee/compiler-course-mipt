#include "language_parsing.h"

#define SUBST_REC(elem1, elem2)                                           \
{                                                                         \
    free(element->elem1);                                                 \
                                                                          \
    element->type  = element->elem2->type;                                \
    element->value = element->elem2->value;                               \
                                                                          \
    bin_tree_elem *temp = element->elem2;                                 \
    element->left  = temp->left;                                          \
    element->right = temp->right;                                         \
                                                                          \
    free(temp);                                                           \
                                                                          \
    (*optimize_state)++;                                                  \
}

#define SUBST_MUL0(elem1, elem2)                                          \
{                                                                         \
    delete_tree_elem(element->elem1);                                     \
    free(element->elem2);                                                 \
                                                                          \
    element->type  = NUM;                                                 \
    element->value = 0;                                                   \
    element->left  = nullptr;                                             \
    element->right = nullptr;                                             \
                                                                          \
    (*optimize_state)++;                                                  \
}                                                                         \

#define SUBST_DIV(elem1, elem2)                                           \
{                                                                         \
    element->elem1->left->type  = element->elem2->type;                   \
    element->elem1->left->value = element->elem2->value;                  \
    element->elem1->left->left  = element->elem2->left;                   \
    element->elem1->left->right = element->elem2->right;                  \
                                                                          \
    free(element->elem2);                                                 \
                                                                          \
    element->type  = element->elem1->type;                                \
    element->value = element->elem1->value;                               \
                                                                          \
    bin_tree_elem *temp = element->elem1;                                 \
    element->left  = temp->left;                                          \
    element->right = temp->right;                                         \
                                                                          \
    free(temp);                                                           \
                                                                          \
    (*optimize_state)++;                                                  \
}                                                                         \


void num_optimize (bin_tree_elem *element, int *optimize_state)
{
    if (L != nullptr)
        num_optimize(L, optimize_state);

    if (element->type == OPER)
    {
        if (L->type == NUM && L->value == 1 && (int) element->value == MUL)
            SUBST_REC(left, right)
        else if (R->type == NUM && R->value == 1 && ((int) element->value == MUL || (int) element->value == DIV || (int) element->value == POW))
            SUBST_REC(right, left)
        else if (L->type == NUM && L->value == 0 && ((int) element->value == ADD || (int) element->value == SUB))
            SUBST_REC(left, right)
        else if (R->type == NUM && R->value == 0 && ((int) element->value == ADD || (int) element->value == SUB))
            SUBST_REC(right, left)
        else if (L->type == NUM && L->value == 0 && ((int) element->value == MUL || (int) element->value == DIV))
            SUBST_MUL0(right, left)
        else if (R->type == NUM && R->value == 0 && (int) element->value == MUL)
            SUBST_MUL0(left, right)
    }

    if (R != nullptr)
        num_optimize(R, optimize_state);
}

void div_optimize (bin_tree_elem *element, int *optimize_state)
{
    if (L != nullptr)
        div_optimize(L, optimize_state);

    if (element->type == OPER && (int) element->value == MUL)
    {
        if (L->type == OPER && (int) L->value == DIV)
        {
            if (L->left->type == NUM && (int) L->left->value == 1 && (int) R->value != DIV)
                SUBST_DIV(left, right)
        }

        if (R->type == OPER && (int) R->value == DIV)
        {
            if (R->left->type == NUM && (int) R->left->value == 1 && (int) L->value != DIV)
                SUBST_DIV(right, left)
        }
    }

    if ((int) element->value == DIV && L->type == VAR && R->type == VAR && (int) L->value == R->value)
    {
        element->type  = NUM;
        element->value = 1;

        free(L);
        free(R);

        (*optimize_state)++;

        L = nullptr;
        R = nullptr;
    }

    if (R != nullptr)
        div_optimize(R, optimize_state);
}

void const_fold_optimize (bin_tree_elem *element, int *optimize_state)
{
    if (L != nullptr)
        const_fold_optimize(L, optimize_state);

    if (element->type == OPER && L->type == NUM && R->type == NUM)
    {
        element->type = NUM;

        switch ((int) element->value)
        {
            case ADD:
            {
                element->value = L->value + R->value;
                break;
            }
            case SUB:
            {
                element->value = L->value - R->value;
                break;
            }
            case MUL:
            {
                element->value = L->value * R->value;
                break;
            }
            case DIV:
            {
                element->value = L->value / R->value;
                break;
            }
            case POW:
            {
                element->value = pow(L->value, R->value);
                break;
            }
            default:
                break;
        }

        free(R);
        free(L);

        R = nullptr;
        L = nullptr;

        (*optimize_state)++;
    }

    if (R != nullptr)
        const_fold_optimize(R, optimize_state);
}

void optimize_tree (bin_tree *tree)
{
    ASSERT_TREE_OK_VOID

    int optimize_state = 0;
    int prev_state     = -1;

    while(optimize_state != prev_state)
    {
        prev_state = optimize_state;

        const_fold_optimize(tree->root, &optimize_state);

        num_optimize(tree->root, &optimize_state);
    }

    optimize_state = 0;
    prev_state     = -1;

    while(optimize_state != prev_state)
    {
        prev_state = optimize_state;

        div_optimize(tree->root, &optimize_state);
    }

    int counter = 0;
    recalc_size_tree(tree->root, &counter);
    tree->tree_size = counter;

    ASSERT_TREE_OK_VOID
}
