#include "list.h"

#define ASSERT_LIST_OK ;            \
    if (error_list(lst))            \
    {                               \
        dump_list(lst);             \
        return;                     \
    }

#define ASSERT_LIST_OK_RET ;        \
    if (error_list(lst))            \
    {                               \
        dump_list(lst);             \
        return -1;                  \
    }

void construct_list (linked_list *lst, int capacity, const char *name)
{
    assert(name);

    if (lst == nullptr)
    {
        lst->error_state = LIST_NULL_PTR;
        dump_list(lst);

        return;
    }

    if (capacity < 0)
    {
        lst->error_state = LIST_NULL_PTR;
        dump_list(lst);

        return;
    }

    lst->curr_size = 0;
    lst->capacity  = capacity;

    lst->list_front = 0;
    lst->list_back  = 0;

    lst->next_free   = 1;
    lst->logic_state = 0;
    lst->error_state = 0;

    lst->elem = (linked_list_el *) calloc(capacity + 1, sizeof(linked_list_el));

    for (int i = 0; i <= lst->capacity; i++)
        lst->elem[i].data = POISON;

    for (int i = 1; i < lst->capacity; i++)
        lst->elem[i].next = i + 1;

    lst->elem[lst->capacity].next = -1;
    lst->elem[0].prev = 0;

    for (int i = 1; i <= lst->capacity; i++)
        lst->elem[i].prev = -1;

    lst->list_name = (char *) calloc(strlen(name) + 1, sizeof(char));
    strcpy(lst->list_name, name);

    ASSERT_LIST_OK
}

void destruct_list (linked_list *lst)
{
    ASSERT_LIST_OK

    lst->curr_size = -1;
    lst->capacity  = -1;

    lst->list_front = -1;
    lst->list_back  = -1;

    lst->next_free   = -1;
    lst->logic_state = -1;

    lst->error_state = 0;

    for (int i = 0; i <= lst->capacity; i++)
    {
        lst->elem[i].data = POISON;
        lst->elem[i].next = -2;
        lst->elem[i].prev = -2;
    }

    free(lst->elem);

    lst->elem = nullptr;
}

void realloc_list (linked_list *lst)
{
    ASSERT_LIST_OK

    if (lst->capacity == 0)
        lst->capacity = LIST_INIT_MAXCOUNT;

    lst->capacity *= LIST_REALLOC_HYSTER;
    lst->elem = (linked_list_el *) realloc(lst->elem, (lst->capacity + 1) * sizeof(linked_list_el));

    for (int i = lst->capacity / LIST_REALLOC_HYSTER + 1; i <= lst->capacity - 1; i++)
    {
        lst->elem[i].data = POISON;
        lst->elem[i].next = i + 1;
        lst->elem[i].prev = -1;
    }

    lst->elem[lst->capacity].data = POISON;
    lst->elem[lst->capacity].next = -1;
    lst->elem[lst->capacity].prev = -1;

    lst->next_free = lst->capacity / LIST_REALLOC_HYSTER + 1;

    ASSERT_LIST_OK
}

int insert_before_list (linked_list *lst, int pos, double value, int logic_state)
{
    ASSERT_LIST_OK_RET

    if (isnan(value))
    {
        lst->error_state = LIST_INVALID_INSERT_NUMBER;
        dump_list(lst);

        return -1;
    }

    if (pos < 1 || pos > lst->capacity)
    {
        lst->error_state = LIST_INVALID_INSERT_POSITION;
        dump_list(lst);

        return -1;
    }

    if (logic_state == LIST_LOGIC_STATE_ON && lst->logic_state != 1)
    {
        lst->error_state = LIST_INABLE_LOGIC_POS;
        dump_list(lst);

        return -1;
    }

    if (lst->elem[pos].prev == -1 && lst->curr_size != 0)
    {
        lst->error_state = LIST_INVALID_INSERT_POSITION;
        return -1;
    }

    if (lst->curr_size == lst->capacity)
        realloc_list(lst);

    int new_pos = lst->next_free;
    lst->next_free = lst->elem[new_pos].next;

    lst->elem[new_pos].data = value;

    if (lst->curr_size == 0)
    {
        lst->elem[new_pos].next = 0;
        lst->elem[new_pos].prev = 0;
        lst->list_front         = new_pos;
        lst->list_back          = new_pos;
        lst->elem[0].next       = new_pos;
        lst->elem[0].prev       = new_pos;

        lst->list_back = new_pos;
    }
    else
    {
        lst->elem[new_pos].prev = lst->elem[pos].prev;
        lst->elem[new_pos].next = pos;
        lst->elem[pos].prev = new_pos;
        lst->elem[lst->elem[new_pos].prev].next = new_pos;

        if (lst->list_front == pos)
           lst->list_front = new_pos;
    }

    lst->curr_size++;

    lst->logic_state = 0;

    ASSERT_LIST_OK_RET

    return new_pos;
}

int insert_after_list (linked_list *lst, int pos, double value, int logic_state)
{
    ASSERT_LIST_OK_RET

    if (isnan(value))
    {
        lst->error_state = LIST_INVALID_INSERT_NUMBER;
        dump_list(lst);

        return -1;
    }

    if (pos < 1 || pos > lst->capacity)
    {
        lst->error_state = LIST_INVALID_INSERT_POSITION;
        dump_list(lst);

        return -1;
    }

    if (logic_state == LIST_LOGIC_STATE_ON && lst->logic_state != 1)
    {
        lst->error_state = LIST_INABLE_LOGIC_POS;
        dump_list(lst);

        return -1;
    }

    if (lst->elem[pos].prev == -1 && lst->curr_size != 0)
    {
        lst->error_state = LIST_INVALID_INSERT_POSITION;
        return -1;
    }

    if (lst->curr_size == lst->capacity)
        realloc_list(lst);

    int new_pos = lst->next_free;
    lst->next_free = lst->elem[new_pos].next;

    lst->elem[new_pos].data = value;

    if (pos != lst->list_back)
        lst->logic_state = 0;

    if (lst->curr_size == 0)
    {
        lst->elem[new_pos].next = 0;
        lst->elem[new_pos].prev = 0;
        lst->list_front         = new_pos;
        lst->list_back          = new_pos;
        lst->elem[0].next       = new_pos;
        lst->elem[0].prev       = new_pos;
    }
    else
    {
        lst->elem[new_pos].prev = pos;
        lst->elem[new_pos].next = lst->elem[pos].next;
        lst->elem[pos].next = new_pos;
        lst->elem[lst->elem[new_pos].next].prev = new_pos;

        if (lst->list_back == pos)
            lst->list_back = new_pos;
    }

    lst->curr_size++;

    ASSERT_LIST_OK_RET

    return new_pos;
}

int push_front_list (linked_list *lst, double value)
{
    ASSERT_LIST_OK_RET

    if (isnan(value))
    {
        lst->error_state = LIST_INVALID_INSERT_NUMBER;
        dump_list(lst);

        return -1;
    }

    if (lst->curr_size == lst->capacity)
        realloc_list(lst);

    int new_pos = lst->next_free;
    lst->next_free = lst->elem[new_pos].next;

    lst->elem[new_pos].data = value;

    if (lst->curr_size == 0)
    {
        lst->elem[new_pos].next = 0;
        lst->elem[new_pos].prev = 0;
        lst->list_front         = new_pos;
        lst->list_back          = new_pos;
        lst->elem[0].next       = new_pos;
        lst->elem[0].prev       = new_pos;
    }
    else
    {
        lst->elem[new_pos].prev = lst->elem[lst->list_front].prev;
        lst->elem[new_pos].next = lst->list_front;
        lst->elem[lst->list_front].prev = new_pos;
        lst->elem[lst->elem[new_pos].prev].next = new_pos;

        lst->list_front = new_pos;
    }

    lst->curr_size++;

    lst->logic_state = 0;

    ASSERT_LIST_OK_RET

    return new_pos;
}

int push_back_list (linked_list *lst, double value)
{
    ASSERT_LIST_OK_RET

    if (isnan(value))
    {
        lst->error_state = LIST_INVALID_INSERT_NUMBER;
        dump_list(lst);

        return -1;
    }

    if (lst->curr_size == lst->capacity)
        realloc_list(lst);

    int new_pos = lst->next_free;
    lst->next_free = lst->elem[new_pos].next;

    lst->elem[new_pos].data = value;

    if (lst->curr_size == 0)
    {
        lst->elem[new_pos].next = 0;
        lst->elem[new_pos].prev = 0;
        lst->list_front         = new_pos;
        lst->list_back          = new_pos;
        lst->elem[0].next       = new_pos;
        lst->elem[0].prev       = new_pos;
    }
    else
    {
        lst->elem[new_pos].prev = lst->list_back;
        lst->elem[new_pos].next = lst->elem[lst->list_back].next;
        lst->elem[lst->list_back].next = new_pos;
        lst->elem[lst->elem[new_pos].next].prev = new_pos;

        lst->list_back = new_pos;
    }

    lst->curr_size++;

    ASSERT_LIST_OK_RET

    return new_pos;
}

void erase_list (linked_list *lst, int pos, int logic_state)
{
    ASSERT_LIST_OK

    if (lst->elem[pos].prev == -1)
    {
        lst->error_state = LIST_ERROR_DELETE;
        dump_list(lst);

        return;
    }

    if (pos < 1 || pos > lst->capacity)
    {
        lst->error_state = LIST_INVALID_DEL_POSITION;
        dump_list(lst);

        return;
    }

    if (logic_state == LIST_LOGIC_STATE_ON && lst->logic_state != 1)
    {
        lst->error_state = LIST_INABLE_LOGIC_POS;
        dump_list(lst);

        return;
    }

    int next_pos = lst->elem[pos].next;
    int prev_pos = lst->elem[pos].prev;

    lst->elem[pos].data = POISON;
    lst->elem[pos].next = lst->next_free;
    lst->elem[pos].prev = -1;

    lst->next_free = pos;

    lst->elem[prev_pos].next = next_pos;
    lst->elem[next_pos].prev = prev_pos;

    if (pos == lst->list_front)
    {
        lst->list_front = lst->elem[0].next;
        lst->logic_state = 0;
    }

    if (pos == lst->list_back)
        lst->list_back = lst->elem[0].prev;

    if (pos != lst->list_front && pos != lst->list_back)
        lst->logic_state = 0;

    lst->curr_size--;

    ASSERT_LIST_OK
}

void pop_front_list (linked_list *lst, int logic_state)
{
    ASSERT_LIST_OK

    if (lst->curr_size == 0)
    {
        lst->error_state = LIST_INVALID_DEL_POSITION;
        dump_list(lst);
        return;
    }

    if (logic_state == LIST_LOGIC_STATE_ON && lst->logic_state != 1)
    {
        lst->error_state = LIST_INABLE_LOGIC_POS;
        dump_list(lst);

        return;
    }

    int next_pos = lst->elem[lst->list_front].next;
    int prev_pos = lst->elem[lst->list_front].prev;

    lst->elem[lst->list_front].data = POISON;
    lst->elem[lst->list_front].next = lst->next_free;
    lst->elem[lst->list_front].prev = -1;

    lst->next_free = lst->list_front;

    lst->elem[prev_pos].next = next_pos;
    lst->elem[next_pos].prev = prev_pos;

    lst->list_front = lst->elem[0].next;
    lst->logic_state = 0;

    if (lst->curr_size == 1)
        lst->list_back = 0;

    lst->curr_size--;

    ASSERT_LIST_OK
}

void pop_back_list (linked_list *lst, int logic_state)
{
    ASSERT_LIST_OK

    if (lst->curr_size == 0)
    {
        lst->error_state = LIST_INVALID_DEL_POSITION;
        dump_list(lst);

        return;
    }

    if (logic_state == LIST_LOGIC_STATE_ON && lst->logic_state != 1)
    {
        lst->error_state = LIST_INABLE_LOGIC_POS;
        dump_list(lst);

        return;
    }

    int next_pos = lst->elem[lst->list_back].next;
    int prev_pos = lst->elem[lst->list_back].prev;

    lst->elem[lst->list_back].data = POISON;
    lst->elem[lst->list_back].next = lst->next_free;
    lst->elem[lst->list_back].prev = -1;

    lst->next_free = lst->list_back;

    lst->elem[prev_pos].next = next_pos;
    lst->elem[next_pos].prev = prev_pos;

    lst->list_back = lst->elem[0].prev;

    if (lst->curr_size == 1)
    {
        lst->list_back   = 0;
        lst->logic_state = 0;
    }

    lst->curr_size--;

    ASSERT_LIST_OK
}

void clear_list (linked_list *lst)
{
    ASSERT_LIST_OK

    for (int i = lst->elem[0].next; i != 0; i = lst->elem[i].next)
    {
        lst->elem[i].data = POISON;
        lst->elem[i].prev = -1;
    }

    for (int i = 1; i < lst->capacity; i++)
        lst->elem[i].next = i + 1;

    lst->elem[0].prev = 0;
    lst->elem[0].next = 0;

    lst->curr_size   = 0;
    lst->list_front  = 0;
    lst->list_back   = 0;
    lst->next_free   = 1;
    lst->logic_state = 0;

    lst->error_state = 0;

    ASSERT_LIST_OK
}

int front_list (linked_list *lst)
{
    ASSERT_LIST_OK_RET

    return lst->list_front;
}

int back_list (linked_list *lst)
{
    ASSERT_LIST_OK_RET

    return lst->list_back;
}

int pos_after_list (linked_list *lst, int pos, int logic_state)
{
    ASSERT_LIST_OK_RET

    if (lst->elem[pos].prev == -1)
    {
        lst->error_state = LIST_ERROR_DELETE;
        dump_list(lst);
        return -1;
    }

    if (pos < 1 || pos > lst->capacity)
    {
        lst->error_state = LIST_INVALID_DEL_POSITION;
        dump_list(lst);
        return -1;
    }

    if (logic_state == LIST_LOGIC_STATE_ON && lst->logic_state != 1)
    {
        lst->error_state = LIST_INABLE_LOGIC_POS;
        dump_list(lst);

        return -1;
    }

    if (pos == lst->list_back)
        return -2;
    else
        return lst->elem[pos].next;
}

int pos_before_list (linked_list *lst, int pos, int logic_state)
{
    ASSERT_LIST_OK_RET

    if (lst->elem[pos].prev == -1)
    {
        lst->error_state = LIST_ERROR_DELETE;
        dump_list(lst);
        return -1;
    }

    if (pos < 1 || pos > lst->capacity)
    {
        lst->error_state = LIST_INVALID_DEL_POSITION;
        dump_list(lst);
        return -1;
    }

    if (logic_state == LIST_LOGIC_STATE_ON && lst->logic_state != 1)
    {
        lst->error_state = LIST_INABLE_LOGIC_POS;
        dump_list(lst);

        return -1;
    }

    if (pos == lst->list_front)
        return -2;
    else
        return lst->elem[pos].prev;
}

int find_logic_pos_list (linked_list *lst, double value)
{
    ASSERT_LIST_OK_RET

    if (isnan(value))
    {
        lst->error_state = LIST_INVALID_INSERT_NUMBER;
        dump_list(lst);

        return -1;
    }

    for (int i = lst->elem[0].next; i != 0; i = lst->elem[i].next)
    {
        if (fabs(lst->elem[i].data - value) < 1e-6)
            return i;
    }

    ASSERT_LIST_OK_RET

    return -1;
}

int find_pos_list (linked_list *lst, int logic_pos)
{
    ASSERT_LIST_OK_RET

    if (logic_pos < 1 || logic_pos > lst->curr_size)
    {
        lst->error_state = LIST_INVALID_INSERT_POSITION;
        dump_list(lst);

        return -1;
    }

    int counter  = 0;
    int pos_data = 0;

    for (pos_data = lst->elem[0].next; pos_data != 0 && counter < logic_pos; pos_data = lst->elem[pos_data].next)
        counter++;

    ASSERT_LIST_OK_RET

    return lst->elem[pos_data].prev;
}

const char *error_text_list (int error)
{
    switch (error) {
        case LIST_LINK_ERROR:
            return "LINK ERROR";
            break;
        case LIST_NEXT_ERR:
            return "SOMEWHERE NEXT IS INVALID";
            break;
        case LIST_PREV_ERR:
            return "SOMEWHERE PREV IS INVALID";
            break;
        case LIST_CONST_RATIO_ERR:
            return "SOMEWHERE PREV OR NEXT IS INVALID";
            break;
        case LIST_NULL_PTR:
            return "NULL LIST PTR";
            break;
        case LIST_NULL_ELEM_PTR:
            return "NULL ELEM PTR";
            break;
        case LIST_INVALID_LIST_SIZE:
            return "INVALID LIST SIZE";
            break;
        case LIST_INVALID_LIST_CAPACITY:
            return "INVALID LIST CAPACITY";
            break;
        case LIST_INVALID_SIZE_CAP_RATIO:
            return "RATIO BETWEEN SIZE AND CAPACITY IS INVALID";
            break;
        case LIST_ERROR_DELETE:
            return "DELETE NON-EXISTANT ELEMENT";
            break;
        case LIST_INVALID_INSERT_NUMBER:
            return "INVALID INSERT NUMBER";
            break;
        case LIST_INVALID_INSERT_POSITION:
            return "INVALID INSERT POSITION VALUE";
            break;
        case LIST_INVALID_DEL_POSITION:
            return "INVALID DELETE POSITION";
            break;
        case LIST_INVALID_INIT_CAPACITY:
            return "INVALID INITIAL CAPACITY";
            break;
        case LIST_FREE_MEMORY_ERROR:
            return "FREE MEMORY ERROR";
            break;
        case LIST_INVALID_POS:
            return "INVALID POSITION";
            break;
        case LIST_INABLE_LOGIC_POS:
            return "INABLE TO WORK WITH LOGIC POSITIONS";
            break;
        default:
            return nullptr;
    }
}

int error_list (linked_list *lst)
{
    if (!lst)
    {
        lst->error_state = LIST_NULL_PTR;
        return LIST_NULL_PTR;
    }
    else if (!lst->elem)
    {
        lst->error_state = LIST_NULL_ELEM_PTR;
        return LIST_NULL_ELEM_PTR;
    }
    else if (isnan(lst->curr_size) || lst->curr_size < 0)
    {
        lst->error_state = LIST_INVALID_LIST_SIZE;
        return LIST_INVALID_LIST_SIZE;
    }
    else if (isnan(lst->capacity) || lst->curr_size < 0)
    {
        lst->error_state = LIST_INVALID_LIST_CAPACITY;
        return LIST_INVALID_LIST_CAPACITY;
    }
    else if (lst->capacity < lst->curr_size)
    {
        lst->error_state = LIST_INVALID_SIZE_CAP_RATIO;
        return LIST_INVALID_SIZE_CAP_RATIO;
    }

    int n_empty_prev = 0;

    for (int i = 1; i <= lst->capacity; i++)
    {
        if (!isnan(lst->elem[i].data))
        {
            if (lst->elem[i].prev == -1)
            {
                lst->error_state = LIST_PREV_ERR;
                return LIST_PREV_ERR;
            }
        }
        else
        {
            if (lst->elem[i].prev != -1)
            {
                lst->error_state = LIST_PREV_ERR;
                return LIST_PREV_ERR;
            }

            n_empty_prev++;
        }
    }

    if (lst->capacity - n_empty_prev != lst->curr_size)
    {
        lst->error_state = LIST_CONST_RATIO_ERR;
        return LIST_CONST_RATIO_ERR;
    }

    if (0 != lst->elem[lst->elem[0].next].prev)
    {
        lst->error_state = LIST_LINK_ERROR;
        return LIST_LINK_ERROR;
    }

    for (int i = lst->elem[0].next; i != 0; i = lst->elem[i].next)
    {
        if (i != lst->elem[lst->elem[i].next].prev)
        {
            lst->error_state = LIST_LINK_ERROR;
            return LIST_LINK_ERROR;
        }
    }

    for (int i = lst->next_free; i != -1; i = lst->elem[i].next)
    {
        if (!isnan(lst->elem[i].data) || lst->elem[i].prev != -1)
        {
            lst->error_state = LIST_FREE_MEMORY_ERROR;
            return LIST_FREE_MEMORY_ERROR;
        }
    }

    return 0;
}

void dump_list (linked_list *lst)
{
    FILE *log = fopen("log_list.txt", "wb");
    assert(log);

    char *curr_time = (char *) calloc(CURRENT_TIME_LENGTH, sizeof(char));
    current_time(curr_time);

    fprintf(log, "List Dump in %s\n", curr_time);

    free(curr_time);

    int error = lst->error_state;

    if (!error)
        fprintf(log, "List (OK) [%p] \"%s\"\n", lst, lst->list_name);
    else if (error == LIST_NULL_PTR || error == LIST_NULL_ELEM_PTR || error == LIST_ERROR_DELETE || error == LIST_INVALID_INSERT_POSITION)
    {
        fprintf(log, "List (ERROR #%d: %s) [%p] \"%s\"\n", error, error_text_list(error), lst, lst->list_name);
        return;
    }
    else
        fprintf(log, "List (ERROR #%d: %s) [%p] \"%s\"\n", error, error_text_list(error), lst, lst->list_name);

    FILE *graphviz = fopen("graph.dot", "wb");
    assert(graphviz);

    fprintf(graphviz, "digraph linked_list {\n");
    fprintf(graphviz, "  node [shape = \"circle\", style = \"filled\", fillcolor = \"blue\", fontcolor = \"#FFFFFF\", margin = \"0.01\"];\n");
    fprintf(graphviz, "  rankdir = \"LR\";\n\n");
    fprintf(graphviz, "  label = \"List Dump\";\n");

    fprintf(log, "size        = %d\n", lst->curr_size);
    fprintf(log, "capacity    = %d\n", lst->capacity);
    fprintf(log, "list_front  = %d\n", lst->list_front);
    fprintf(log, "list_back   = %d\n", lst->list_back);
    fprintf(log, "next_free   = %d\n", lst->next_free);
    fprintf(log, "logic_state = %d\n", lst->logic_state);

    fprintf(log, "Data[%p]:\n", lst->elem);

    fprintf(log, "*[%d] data = %lg, next = %d, prev = %d\n", 0, lst->elem[0].data, lst->elem[0].next, lst->elem[0].prev);

    for (int i = 1; i <= lst->capacity; i++)
        fprintf(log, "*[%d] data = %lg, next = %d, prev = %d\n", i, lst->elem[i].data, lst->elem[i].next, lst->elem[i].prev);

    char temp_name[10] = {0};

    int counter = 0;

    for (int i = lst->elem[0].next; i != 0; i = lst->elem[i].next)
    {
        strcpy(temp_name, "");

        if (i == lst->list_front)
            strcpy(temp_name, "\\nFront");

        else if (i == lst->list_back)
            strcpy(temp_name, "\\nBack");

        counter++;

        fprintf(graphviz, "  \"Vertex %d\"[shape = \"record\", label = \"%lg%s | {prev\\n%d | <f0> pos\\n%d| next\\n%d} | number\\n%d\"];\n", i, lst->elem[i].data, temp_name, lst->elem[i].prev, i, lst->elem[i].next, counter);
    }

    for (int i = lst->elem[0].next; lst->elem[i].next != 0; i = lst->elem[i].next)
    {
        if (lst->elem[lst->elem[i].next].prev != i)
        {
            fprintf(graphviz, "  edge [color=\"#FE6200\"];\n");
            fprintf(graphviz, "  \"Vertex %d\"->\"Vertex %d\";\n", i, lst->elem[i].next);
            fprintf(graphviz, "  edge [color=\"#000000\"];\n");
        }
        else
            fprintf(graphviz, "  \"Vertex %d\"->\"Vertex %d\";\n", i, lst->elem[i].next);
    }

    for (int i = lst->elem[lst->list_front].next; i != 0; i = lst->elem[i].next)
    {
        if (lst->elem[lst->elem[i].prev].next != i)
            {
            fprintf(graphviz, "  edge [color=\"#FE6200\"];\n");
            fprintf(graphviz, "  \"Vertex %d\"->\"Vertex %d\";\n", i, lst->elem[i].prev);
            fprintf(graphviz, "  edge [color=\"#000000\"];\n");
        }
        else
            fprintf(graphviz, "  \"Vertex %d\"->\"Vertex %d\";\n", i, lst->elem[i].prev);
    }

    fprintf(log, "\n");

    fprintf(graphviz, "}");

    fclose(graphviz);
    fclose(log);

    system("dot -Tpdf -o graph.pdf graph.dot");

    system("start graph.pdf");
    system("start log_list.txt");
}

void exch_phys_list (linked_list *lst, int pos1, int pos2)
{
    if (pos1 < 1 || pos1 > lst->capacity)
    {
        lst->error_state = LIST_INVALID_POS;
        dump_list(lst);
        return;
    }

    if (pos2 < 1 || pos2 > lst->capacity)
    {
        lst->error_state = LIST_INVALID_POS;
        dump_list(lst);
        return;
    }

    if (lst->list_front == pos1)
        lst->list_front = pos2;
    else if (lst->list_front == pos2)
        lst->list_front = pos1;

    if (lst->list_back == pos1)
        lst->list_back = pos2;
    else if (lst->list_back == pos2)
        lst->list_back = pos1;

    lst->elem[lst->elem[pos1].prev].next = pos2;
    lst->elem[lst->elem[pos2].prev].next = pos1;

    lst->elem[lst->elem[pos1].next].prev = pos2;
    lst->elem[lst->elem[pos2].next].prev = pos1;

    double temp = lst->elem[pos1].data;
    lst->elem[pos1].data = lst->elem[pos2].data;
    lst->elem[pos2].data = temp;

    temp = lst->elem[pos1].next;
    lst->elem[pos1].next = lst->elem[pos2].next;
    lst->elem[pos2].next = temp;

    temp = lst->elem[pos1].prev;
    lst->elem[pos1].prev = lst->elem[pos2].prev;
    lst->elem[pos2].prev = temp;
}

void exch_logic_list (linked_list *lst, int pos1, int pos2)
{
    if (pos1 < 1 || pos1 > lst->capacity)
    {
        lst->error_state = LIST_INVALID_POS;
        dump_list(lst);
        return;
    }

    if (pos2 < 1 || pos2 > lst->capacity)
    {
        lst->error_state = LIST_INVALID_POS;
        dump_list(lst);
        return;
    }

    double temp = lst->elem[pos1].data;
    lst->elem[pos1].data = lst->elem[pos2].data;
    lst->elem[pos2].data = temp;
}

int elem_comp_list (const void *elem1, const void *elem2)
{
    assert(elem1);
    assert(elem2);

    if (((linked_list_el *)elem1)->prev == -1 && ((linked_list_el *)elem2)->prev == -1)
        return 0;

    if (((linked_list_el *)elem2)->prev == -1)
        return -1;

    if (((linked_list_el *)elem1)->prev == -1)
        return 1;

    return ((linked_list_el *)elem1)->data - ((linked_list_el *)elem2)->data;
}

void sort_next_list (linked_list *lst)
{
    ASSERT_LIST_OK

    linked_list_el *new_elem = (linked_list_el *) calloc(lst->capacity + 1, sizeof(linked_list_el));

    new_elem[0].data = POISON;
    new_elem[0].next = 1;
    new_elem[0].prev = lst->curr_size;

    int counter = 0;

    for (int i = lst->elem[0].next; i != 0; i = lst->elem[i].next)
    {
        counter++;

        new_elem[counter].data = lst->elem[i].data;
    }

    for (int i = 1; i <= lst->curr_size - 1; i++)
    {
        new_elem[i].next = i + 1;
        new_elem[i].prev = i - 1;
    }

    new_elem[lst->curr_size].next = 0;
    new_elem[lst->curr_size].prev = lst->curr_size - 1;

    for (int i = lst->curr_size + 1; i <= lst->capacity - 1; i++)
    {
        new_elem[i].data = POISON;
        new_elem[i].next = i + 1;
        new_elem[i].prev = -1;
    }

    new_elem[lst->capacity].next = -1;
    new_elem[lst->capacity].prev = -1;
    new_elem[lst->capacity].data = POISON;

    lst->next_free  = lst->curr_size + 1;
    lst->list_front = 1;
    lst->list_back  = lst->curr_size;

    free(lst->elem);

    lst->elem = new_elem;

    lst->logic_state = 1;

    ASSERT_LIST_OK
}

void sort_list (linked_list *lst)
{
    ASSERT_LIST_OK

    sort_next_list(lst);

    qsort_list(lst, 1, lst->curr_size, elem_comp_list);

    ASSERT_LIST_OK

}

void qsort_list (linked_list *lst, int l_0, int r_0, int (*list_elem_comp)(const void*, const void*))
{
    if (r_0 - l_0 <= 0)
        return;

    int piv = (l_0 + r_0) / 2;
    int l = l_0;
    int r = r_0;

    const void *piv_elem = (const void *) &lst->elem[piv];

    do
    {
        while (elem_comp_list(piv_elem, (const void *) &lst->elem[l]) > 0)
            l++;
        while (elem_comp_list((const void *) &lst->elem[r], piv_elem) > 0)
            r--;

        if (l <= r)
            exch_logic_list(lst, l++, r--);

    } while (l <= piv && r >= piv);

    if (l_0 < r)
        qsort_list(lst, l_0, r, list_elem_comp);
    if (r_0 > l)
        qsort_list(lst, l, r_0, list_elem_comp);
}


