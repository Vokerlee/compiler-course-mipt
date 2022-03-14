#include "p_list.h"

#define ASSERT_P_LIST_OK                    \
    if (error_p_list(lst))                  \
    {                                       \
        dump_p_list(lst);                   \
        return;                             \
    }

#define ASSERT_P_LIST_OK_RET                \
    if (error_p_list(lst))                  \
    {                                       \
        dump_p_list(lst);                   \
        return nullptr;                     \
    }

#define ASSERT_P_LIST_OK_INT                \
    if (error_p_list(lst))                  \
    {                                       \
        dump_p_list(lst);                   \
        return -1;                          \
    }

void construct_p_list (p_linked_list *lst, const char *name)
{
    assert(lst);
    assert(name);

    lst->curr_size  = 0;

    lst->list_front = nullptr;
    lst->list_back  = nullptr;

    lst->error_state = 0;

    lst->list_name = (char *) calloc(strlen(name) + 1, sizeof(char));
    strcpy(lst->list_name, name);
}

void destruct_p_list (p_linked_list *lst)
{
    ASSERT_P_LIST_OK

    p_linked_list_el *pointer = nullptr;

    for (pointer = lst->list_back; pointer->prev != nullptr; )
    {
        pointer = pointer->prev;

        free(pointer->next);
        pointer->next = nullptr;
    }

    free(pointer);
    lst->list_front = nullptr;
    lst->list_back  = nullptr;

    lst->curr_size  = -1;

    lst->error_state = 0;

}

p_linked_list_el *insert_before_p_list (p_linked_list *lst, p_linked_list_el *pos, double value)
{
    ASSERT_P_LIST_OK_RET

    if (isnan(value))
    {
        lst->error_state = P_LIST_INVALID_INSERT_NUMBER;
        dump_p_list(lst);

        return nullptr;
    }
    else if (pos == nullptr && lst->curr_size != 0)
    {
        lst->error_state = P_LIST_INVALID_INSERT_POSITION;
        dump_p_list(lst);

        return nullptr;
    }

    p_linked_list_el *temp = (p_linked_list_el *) calloc(1, sizeof(p_linked_list_el));

    if (lst->curr_size == 0)
    {
        temp->data = value;
        temp->next = nullptr;
        temp->prev = nullptr;

        lst->list_front = temp;
        lst->list_back  = temp;
    }
    else
    {
        temp->data = value;
        temp->next = pos;
        temp->prev = pos->prev;

        if (pos->prev != nullptr)
        {
            (pos->prev)->next = temp;
        }

        pos->prev = temp;

        if (lst->list_front == pos)
            lst->list_front = temp;
    }

    lst->curr_size++;

    ASSERT_P_LIST_OK_RET

    return temp;
}

p_linked_list_el *insert_after_p_list (p_linked_list *lst, p_linked_list_el *pos, double value)
{
    ASSERT_P_LIST_OK_RET

    if (isnan(value))
    {
        lst->error_state = P_LIST_INVALID_INSERT_NUMBER;
        dump_p_list(lst);

        return nullptr;
    }
    else if (pos == nullptr && lst->curr_size != 0)
    {
        lst->error_state = P_LIST_INVALID_INSERT_POSITION;
        dump_p_list(lst);

        return nullptr;
    }

    p_linked_list_el *temp = (p_linked_list_el *) calloc(1, sizeof(p_linked_list_el));

    if (lst->curr_size == 0)
    {
        temp->data = value;
        temp->next = nullptr;
        temp->prev = nullptr;

        lst->list_front = temp;
        lst->list_back  = temp;
    }
    else
    {
        temp->data = value;
        temp->next = pos->next;
        temp->prev = pos;

        if (pos->next != nullptr)
        {
            (pos->next)->prev = temp;
        }

        pos->next = temp;

        if (lst->list_back == pos)
            lst->list_back = temp;
    }

    lst->curr_size++;

    ASSERT_P_LIST_OK_RET

    return temp;
}

p_linked_list_el *push_front_p_list (p_linked_list *lst, double value)
{
    ASSERT_P_LIST_OK_RET

    if (isnan(value))
    {
        lst->error_state = P_LIST_INVALID_INSERT_NUMBER;
        dump_p_list(lst);

        return nullptr;
    }

    p_linked_list_el *temp = (p_linked_list_el *) calloc(1, sizeof(p_linked_list_el));

    if (lst->curr_size == 0)
    {
        temp->data = value;
        temp->next = nullptr;
        temp->prev = nullptr;

        lst->list_front = temp;
        lst->list_back  = temp;
    }
    else
    {
        temp->data = value;
        temp->next = lst->list_front;
        temp->prev = nullptr;
        (lst->list_front)->prev = temp;

        lst->list_front = temp;
    }

    lst->curr_size++;

    ASSERT_P_LIST_OK_RET

    return temp;
}

p_linked_list_el *push_back_p_list (p_linked_list *lst, double value)
{
    ASSERT_P_LIST_OK_RET

    if (isnan(value))
    {
        lst->error_state = P_LIST_INVALID_INSERT_NUMBER;
        dump_p_list(lst);

        return nullptr;
    }

    p_linked_list_el *temp = (p_linked_list_el *) calloc(1, sizeof(p_linked_list_el));

    if (lst->curr_size == 0)
    {
        temp->data = value;
        temp->next = nullptr;
        temp->prev = nullptr;

        lst->list_front = temp;
        lst->list_back  = temp;
    }
    else
    {
        temp->data = value;
        temp->next = nullptr;
        temp->prev = lst->list_back;
        (lst->list_back)->next = temp;

        lst->list_back = temp;
    }

    lst->curr_size++;

    ASSERT_P_LIST_OK_RET

    return temp;
}

void erase_p_list (p_linked_list *lst, p_linked_list_el *pos)
{
    ASSERT_P_LIST_OK

    if (pos == nullptr)
    {
        lst->error_state = P_LIST_ERROR_DELETE;
        dump_p_list(lst);

        return;
    }

    p_linked_list_el *next_pos = pos->next;
    p_linked_list_el *prev_pos = pos->prev;

    if (lst->curr_size == 1)
    {
        lst->list_front = nullptr;
        lst->list_back  = nullptr;
    }
    else if (pos == lst->list_front)
    {
        lst->list_front = pos->next;
    }
    else if (pos == lst->list_back)
    {
        lst->list_back = pos->prev;
    }

    if (next_pos != nullptr)
        next_pos->prev = prev_pos;

    if (prev_pos != nullptr)
        prev_pos->next = next_pos;

    free(pos);

    lst->curr_size--;

    ASSERT_P_LIST_OK
}

void clear_p_list (p_linked_list *lst)
{
    ASSERT_P_LIST_OK

    p_linked_list_el *pointer = nullptr;

    for (pointer = lst->list_back; pointer->prev != nullptr; )
    {
        pointer = pointer->prev;

        free(pointer->next);
        pointer->next = nullptr;
    }

    free(pointer);
    lst->list_front = nullptr;

    lst->curr_size  = 0;
    lst->list_back  = nullptr;

    ASSERT_P_LIST_OK
}

p_linked_list_el *front_p_list (p_linked_list *lst)
{
    ASSERT_P_LIST_OK_RET

    return lst->list_front;
}

p_linked_list_el *back_p_list (p_linked_list *lst)
{
    ASSERT_P_LIST_OK_RET

    return lst->list_back;
}

p_linked_list_el *pos_after_p_list (p_linked_list *lst, p_linked_list_el *pos)
{
    ASSERT_P_LIST_OK_RET

    if (pos == nullptr)
    {
        lst->error_state = P_LIST_ERROR_DELETE;
        dump_p_list(lst);

        return nullptr;
    }

    return pos->next;
}

p_linked_list_el *pos_before_p_list (p_linked_list *lst, p_linked_list_el *pos)
{
    ASSERT_P_LIST_OK_RET

    if (pos == nullptr)
    {
        lst->error_state = P_LIST_ERROR_DELETE;
        dump_p_list(lst);

        return nullptr;
    }

    return pos->prev;
}

int find_index_p_list (p_linked_list *lst, double value)
{
    ASSERT_P_LIST_OK_INT

    if (isnan(value))
    {
        lst->error_state = P_LIST_INVALID_INSERT_NUMBER;
        dump_p_list(lst);

        return -1;
    }

    int counter = 0;

    for (p_linked_list_el *pointer = lst->list_front; pointer != nullptr; pointer = pointer->next)
    {
        counter++;
        if (fabs(pointer->data - value) < 1e-6)
            return counter;
    }

    ASSERT_P_LIST_OK_INT

    return -1;
}

double find_value_p_list (p_linked_list *lst, int pos)
{
    ASSERT_P_LIST_OK_INT

    if (pos < 1 || pos > lst->curr_size)
    {
        lst->error_state = P_LIST_INVALID_INSERT_POSITION;
        dump_p_list(lst);

        return POISON;
    }

    int counter = 0;
    p_linked_list_el *pointer = nullptr;

    for (pointer = lst->list_front; pointer != nullptr; pointer = pointer->next)
    {
        counter++;
        if (counter == pos)
            break;
    }

    ASSERT_P_LIST_OK_INT

    return pointer->data;
}

const char *error_text_p_list (int error)
{
    switch (error) {
        case P_LIST_LINK_ERROR:
            return "LINK ERROR";
            break;
        case P_LIST_NEXT_ERR:
            return "SOMEWHERE NEXT IS INVALID";
            break;
        case P_LIST_PREV_ERR:
            return "SOMEWHERE PREV IS INVALID";
            break;
        case P_LIST_CONST_RATIO_ERR:
            return "SOMEWHERE PREV OR NEXT IS INVALID";
            break;
        case P_LIST_NULL_PTR:
            return "NULL LIST PTR";
            break;
        case P_LIST_NULL_ELEM_PTR:
            return "NULL ELEM PTR";
            break;
        case P_LIST_INVALID_LIST_SIZE:
            return "INVALID LIST SIZE";
            break;
        case P_LIST_INVALID_LIST_CAPACITY:
            return "INVALID LIST CAPACITY";
            break;
        case P_LIST_INVALID_SIZE_CAP_RATIO:
            return "RATIO BETWEEN SIZE AND CAPACITY IS INVALID";
            break;
        case P_LIST_ERROR_DELETE:
            return "DELETE NON-EXISTANT ELEMENT";
            break;
        case P_LIST_INVALID_INSERT_NUMBER:
            return "INVALID INSERT NUMBER";
            break;
        case P_LIST_INVALID_INSERT_POSITION:
            return "INVALID INSERT POSITION VALUE";
            break;
        case P_LIST_INVALID_DEL_POSITION:
            return "INVALID DELETE POSITION";
            break;
        case P_LIST_INVALID_INIT_CAPACITY:
            return "INVALID INITIAL CAPACITY";
            break;
        case P_LIST_FREE_MEMORY_ERROR:
            return "FREE MEMORY ERROR";
            break;
        default:
            return nullptr;
    }
}

int error_p_list (p_linked_list *lst)
{
    assert(lst);

    if (!lst)
    {
        lst->error_state = P_LIST_NULL_PTR;
        return P_LIST_NULL_PTR;
    }
    else if (isnan(lst->curr_size) || lst->curr_size < 0)
    {
        lst->error_state = P_LIST_INVALID_LIST_SIZE;
        return P_LIST_INVALID_LIST_SIZE;
    }
    else if ((lst->list_front == nullptr || lst->list_back == nullptr) && lst->curr_size != 0)
    {
        lst->error_state = P_LIST_INVALID_LIST_SIZE;
        return P_LIST_INVALID_LIST_SIZE;
    }

    if (lst->curr_size > 0)
    {
        for (p_linked_list_el *pointer = lst->list_front; pointer->next != nullptr; pointer = pointer->next)
        {
            if ((pointer->next)->prev != pointer)
            {
                lst->error_state = P_LIST_LINK_ERROR;
                return P_LIST_LINK_ERROR;
            }
        }
    }

    return 0;
}

void dump_p_list (p_linked_list *lst)
{
    FILE *log = fopen("log_list.txt", "wb");
    assert(log);

    int error = lst->error_state;

    if (!error)
        fprintf(log, "List (OK) [%p] \"%s\"\n", (void *) lst, lst->list_name);
    else if (error == P_LIST_NULL_PTR || error == P_LIST_NULL_ELEM_PTR || error == P_LIST_ERROR_DELETE || error == P_LIST_INVALID_INSERT_POSITION)
    {
        fprintf(log, "List (ERROR #%d: %s) [%p] \"%s\"\n", error, error_text_p_list(error), (void *) lst, lst->list_name);
        return;
    }
    else
        fprintf(log, "List (ERROR #%d: %s) [%p] \"%s\"\n", error, error_text_p_list(error), (void *) lst, lst->list_name);

    FILE *graphviz = fopen("graph.dot", "wb");
    assert(graphviz);

    fprintf(graphviz, "digraph linked_list {\n");
    fprintf(graphviz, "  node [shape = \"circle\", style = \"filled\", fillcolor = \"blue\", fontcolor = \"#FFFFFF\", margin = \"0.01\"];\n");
    fprintf(graphviz, "  edge [style = \"dashed\"];\n");
    fprintf(graphviz, "  rankdir = \"LR\";\n\n");
    fprintf(graphviz, "  label = \"List Dump\";\n");

    fprintf(log, "size       = %d\n", lst->curr_size);
    fprintf(log, "list_front = %p\n", (void *) lst->list_front);
    fprintf(log, "list_back  = %p\n", (void *) lst->list_back);


    for (p_linked_list_el *pointer = lst->list_front; pointer != nullptr; pointer = pointer->next)
        fprintf(log, "[%p] data = %lg, next = %p, prev = %p\n", (void *) pointer, pointer->data, (void *) pointer->next, (void *) pointer->prev);

    int counter = 0;

    for (p_linked_list_el *pointer = lst->list_front; pointer != nullptr; pointer = pointer->next)
    {
        counter++;
        fprintf(graphviz, "  \"V%p\"[shape = \"record\", fillcolor=\"blue\", label = \"%lg | {next\\n(%p) | previous\\n(%p)} |<f0> %d\"];\n",
                (void *) pointer, pointer->data, (void *) pointer->next, (void *) pointer->prev, counter);
    }

    for (p_linked_list_el *pointer = lst->list_back; pointer != nullptr; pointer = pointer->prev)
    {
        if (pointer->next != nullptr)
            fprintf(graphviz, "  \"V%p\"->\"V%p\";\n", (void *) pointer,(void *)  pointer->next);
    }

    for (p_linked_list_el *pointer = lst->list_front; pointer != nullptr; pointer = pointer->next)
    {
        if (pointer->prev != nullptr)
            fprintf(graphviz, "  \"V%p\"->\"V%p\";\n", (void *) pointer, (void *) pointer->prev);
    }

    fprintf(log, "\n");
    fclose(log);

    fprintf(graphviz, "}");
    fclose(graphviz);

    system("dot -Tpdf -o graph.pdf graph.dot");
}
