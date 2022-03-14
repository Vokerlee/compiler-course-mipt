#include "stack.h"

int main()
{
    FILE *check = fopen("example.txt", "wb");

    CONSTRUCT_STACK(stk, 5);

    for (int i = 0; i < 7; i++)
        push_stack(&stk, i);

    dump_stack(&stk, check);

    printf("%lg\n", top_stack(&stk));

    stk.counter = 2;

    destruct_stack(&stk);

    system("pause");

    fclose(check);

    return 0;
}