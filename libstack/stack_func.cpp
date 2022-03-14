#include "stack.h"

void construct_stack (stack_t *stk, int capacity, const char *name)
{
    if (stk == nullptr || double_constr_error_stack(stk) != 0)
    {
        if (stk == nullptr)
            stk->error_state = STACK_NULL_PTR;

        FILE *log = fopen("log_stack.txt", "ab");
        dump_stack(stk, log);
        fclose(log);

        return;
    }

    stk->stack_name = (char *) calloc(STACK_MAX_NAME_LENGTH + 1, sizeof(char));
    strcpy(stk->stack_name, name);

    if (capacity < 0)
    {
        stk->error_state = STACK_INVALID_CAPACITY;

        FILE *log = fopen("log_stack.txt", "ab");
        dump_stack(stk, log);
        fclose(log);

        return;
    }

    stk->canary_left  = STACK_CANARY_LEFT;
    stk->canary_right = STACK_CANARY_RIGHT;

    stk->counter     = 0;
    stk->capacity    = capacity;
    stk->hash_number = 0;

    stk->data = (data_t*) calloc(capacity + 1, sizeof(data_t));

    if (stk->data == nullptr)
    {
        stk->error_state = STACK_NULL_DATA_PTR;

        FILE *log = fopen("log_stack.txt", "ab");
        dump_stack(stk, log);
        fclose(log);

        return;
    }

    *((int *) stk->data) = STACK_DATA_CANARY_LEFT;

    int *pointer = (int *) stk->data;
    pointer++;
    stk->data = (data_t *) pointer;

    *((int *) (stk->data + capacity)) = STACK_DATA_CANARY_RIGHT;

    for (int i = 0; i < capacity; i++)
        stk->data[i] = POISON;

    update_hash_stack(stk);

    STACK_ASSERT_OK
}

void destruct_stack (stack_t *stk)
{
    STACK_ASSERT_OK

    *((int*) ((char*) stk->data - sizeof(int))) = -1;
    *((int*) (stk->data + stk->capacity))       = -2;

    for (int i = 0; i < stk->capacity; i++)
        stk->data[i] = INT_MAX;

    free((char *) stk->data - sizeof(int));
    free(stk->stack_name);

    stk->data       = nullptr;
    stk->stack_name = nullptr;

    stk->canary_left  = -3;
    stk->canary_right = -4;

    stk->counter     = INT_MAX;
    stk->capacity    = INT_MAX;
    stk->hash_number = INT_MAX;
    stk->error_state = INT_MAX;
}

void push_stack (stack_t *stk, double value)
{
    STACK_ASSERT_OK

    if (isnan(value) != 0)
    {
        stk->error_state = STACK_INVALID_PUSH;

        FILE *log = fopen("log_stack.txt", "ab");
        dump_stack(stk, log);
        fclose(log);

        return;
    }

    if (stk->counter == stk->capacity)
        realloc_stack(stk, STACK_ADD_ALLOC);

    stk->data[stk->counter++] = value;

    update_hash_stack(stk);

    STACK_ASSERT_OK
}

double pop_stack (stack_t *stk)
{
    STACK_POP_STATE = ON;
    STACK_ASSERT_POP_OK

    if (stk->counter <= stk->capacity / STACK_REALLOC_HYSTER / 2 + 1)
        realloc_stack(stk, STACK_RED_ALLOC);

    stk->counter--;

    double value = stk->data[stk->counter];
    stk->data[stk->counter] = POISON;

    STACK_POP_STATE = OFF;

    update_hash_stack(stk);

    STACK_ASSERT_POP_OK

    return value;
}

double top_stack (stack_t *stk)
{
    STACK_ASSERT_POP_OK

    return stk->data[stk->counter - 1];
}

int empty_stack (stack_t *stk)
{
    if (error_stack(stk))
    {
        FILE *log = fopen("log_stack.txt", "ab");
        dump_stack(stk, log);
        fclose(log);

        return -1;
    }

    if (stk->counter != 0)
        return 0;

    return 1;
}

void realloc_stack (stack_t *stk, int regime)
{
    STACK_ASSERT_OK

    if (regime == STACK_ADD_ALLOC)
    {
        if (stk->capacity == 0)
            stk->capacity = STACK_INIT_MAXCOUNT;

        stk->capacity *= STACK_REALLOC_HYSTER;
        stk->data = (data_t *) realloc((char *) stk->data - sizeof(int), (stk->capacity + 1) * sizeof(data_t));

        int *pointer = (int *) stk->data;
        pointer++;
        stk->data = (data_t *) pointer;

        for (int i = stk->capacity / STACK_REALLOC_HYSTER + 1; i < stk->capacity; i++)
            stk->data[i] = POISON;

        *((int *) (stk->data + stk->capacity)) = STACK_DATA_CANARY_RIGHT;

    }
    else if (regime == STACK_RED_ALLOC && stk->capacity >= STACK_INIT_MAXCOUNT)
    {
        stk->capacity /= STACK_REALLOC_HYSTER;
        stk->data = (data_t *) realloc((char *) stk->data - sizeof(int), (stk->capacity + 1) * sizeof(data_t));

        int *pointer = (int *) stk->data;
        pointer++;
        stk->data = (data_t *) pointer;

        *((int *) (stk->data + stk->capacity)) = STACK_DATA_CANARY_RIGHT;
    }

    update_hash_stack(stk);

    STACK_ASSERT_OK
}

int error_stack (stack_t *stk)
{
    if (stk == nullptr)
        return STACK_NULL_PTR;
    if (stk->error_state != 0)
        return stk->error_state;
    else if (stk->data == nullptr)
    {
        stk->error_state = STACK_NULL_DATA_PTR;
        return STACK_NULL_DATA_PTR;
    }
    else if (stk->capacity < 0)
    {
        stk->error_state = STACK_INVALID_CAPACITY;
        return STACK_INVALID_CAPACITY;
    }
    else if (stk->counter > stk->capacity)
    {
        stk->error_state = STACK_INVALID_COUNTER;
        return STACK_INVALID_COUNTER;
    }
    else if (recalculate_hash_stack(stk) != stk->hash_number)
    {
        stk->error_state = STACK_HACKED;
        return STACK_HACKED;
    }
    else if (stk->canary_right != STACK_CANARY_RIGHT)
    {
        stk->error_state = STACK_CANARY_RIGHT_ERR;
        return STACK_CANARY_RIGHT_ERR;
    }
    else if (stk->canary_left != STACK_CANARY_LEFT)
    {
        stk->error_state = STACK_CANARY_LEFT_ERR;
        return STACK_CANARY_LEFT_ERR;
    }
    else if (*((int*) ((char*) stk->data - sizeof(int))) != STACK_DATA_CANARY_LEFT)
    {
        stk->error_state = STACK_CANARY_DATA_LEFT_ERR;
        return STACK_CANARY_DATA_LEFT_ERR;
    }
    else if (*((int*) (stk->data + stk->capacity)) != STACK_DATA_CANARY_RIGHT)
    {
        stk->error_state = STACK_CANARY_DATA_RIGHT_ERR;
        return STACK_CANARY_DATA_RIGHT_ERR;
    }
    else if (STACK_POP_STATE == ON && stk->counter == 0)
    {
        stk->error_state = STACK_EMPTY_POP;
        return STACK_EMPTY_POP;
    }
    else
        return 0;
}

const char *error_text (int error)
{
    switch (error) {
        case STACK_NULL_PTR:
            return "NULL STACK PTR";
        case STACK_DOUBLE_CONSTR:
            return "CONSTRUCTOR WORKS TWICE";
        case STACK_NULL_DATA_PTR:
            return "NULL DATA PTR";
        case STACK_INVALID_CAPACITY:
            return "INVALID CAPACITY";
        case STACK_INVALID_COUNTER:
            return "INVALID COUNTER";
        case STACK_CANARY_RIGHT_ERR:
            return "STRUCT CANARY_RIGHT: SOMETHING TRIED TO GET INTO STRUCT OUTSIDE";
        case STACK_CANARY_LEFT_ERR:
            return "STRUCT CANARY_LEFT: SOMETHING TRIED TO GET INTO STRUCT OUTSIDE";
        case STACK_CANARY_DATA_LEFT_ERR:
            return "DATA CANARY_LEFT: SOMETHING TRIED TO GET INTO STRUCT OUTSIDE";
        case STACK_CANARY_DATA_RIGHT_ERR:
            return "DATA CANARY_RIGHT: SOMETHING TRIED TO GET INTO STRUCT OUTSIDE";
        case STACK_EMPTY_POP:
            return "POP UNEXISTABLE ELEMENT";
        case STACK_HACKED:
            return "STACK IS BEING TRIED TO HACK";
        case STACK_INVALID_PUSH:
            return "INVALID PUSH";
        default:
            return "GLOBAL STACK ERROR: ERROR WASN'T RECOGNIZED";
    }
}

int double_constr_error_stack (stack_t *stk)
{
    if (stk->canary_left != STACK_CANARY_LEFT || stk->canary_right != STACK_CANARY_RIGHT)
        return 0;
    else if (*((int*) (stk->data + stk->capacity)) != STACK_DATA_CANARY_RIGHT || *((int*) ((char*) stk->data - sizeof(int))) != STACK_DATA_CANARY_LEFT)
        return 0;
    else
    {
        stk->error_state = STACK_DOUBLE_CONSTR;
        return STACK_DOUBLE_CONSTR;
    }
}

void dump_stack (stack_t *stk, FILE *log)
{
    assert(log);

    char *curr_time = (char *) calloc(CURRENT_TIME_LENGTH, sizeof(char));
    current_time(curr_time);

    fprintf(log, "Stack Dump in %s\n", curr_time);

    free(curr_time);

    int error = stk->error_state;

    if (error == STACK_NULL_PTR || error == STACK_NULL_DATA_PTR || error == STACK_INVALID_CAPACITY || error == STACK_INVALID_PUSH || error == STACK_DOUBLE_CONSTR)
    {
        fprintf(log, "Stack (ERROR #%d: %s) [%p] \"%s\"\n", error, error_text(error), (void *) stk, stk->stack_name);
        return;
    }

    if (error == 0)
        fprintf(log, "Stack (OK) [%p] \"%s\"\n", (void *) stk, stk->stack_name);
    else
        fprintf(log, "Stack (ERROR #%d: %s) [%p] \"%s\"\n", error, error_text(error), (void *) stk, stk->stack_name);

    fprintf(log, "counter  = %d\n", stk->counter);
    fprintf(log, "maxcount = %d\n", stk->capacity);
    fprintf(log, "Data[%p]\n", (void *) stk->data);

    if (error != STACK_INVALID_COUNTER && error != STACK_HACKED)
    {
        for (int i = 0; i < stk->counter; i++)
            fprintf(log, "*[%d] = %" SPECIFICATOR "\n", i + 1, stk->data[i]);

        for (int i = stk->counter; i < stk->capacity; i++)
            fprintf(log, "[%d] = %" SPECIFICATOR "\n", i + 1, stk->data[i]);
    }
    else if (error == STACK_HACKED || error == STACK_INVALID_COUNTER)
    {
        for (int i = 0; i < stk->capacity; i++)
            fprintf(log, "?[%d] = %" SPECIFICATOR "\n", i + 1, stk->data[i]);
    }

    fprintf(log, "\n");
}

void update_hash_stack (stack_t *stk)
{
    char *pointer = (char *) stk;

    int temp = 1;

    pointer += 8;

    for (int i = 0; i < sizeof(stack_t) - 8; i++)
    {
        temp += temp * (i + 1) * (*pointer + 6);
        pointer++;
    }

    for (int i = 0; i < stk->counter; i++)
        temp += temp * ((int) stk->data[i] >> 1) | ((int) stk->data[i] << 1);

    int stack_length = strlen(stk->stack_name);

    for (int i = 0; i < stack_length; i++)
        temp += temp * stk->stack_name[i];

    stk->hash_number = temp;
}

int recalculate_hash_stack (stack_t *stk)
{
    char *pointer = (char *) stk;

    int temp = 1;

    pointer += 8;

    for (int i = 0; i < sizeof(stack_t) - 8; i++)
    {
        temp += temp * (i + 1) * (*pointer + 6);
        pointer++;
    }

    for (int i = 0; i < stk->counter; i++)
        temp += temp * ((int) stk->data[i] >> 1) | ((int) stk->data[i] << 1);

    int stack_length = strlen(stk->stack_name);

    for (int i = 0; i < stack_length; i++)
        temp += temp * stk->stack_name[i];

    return temp;
}

