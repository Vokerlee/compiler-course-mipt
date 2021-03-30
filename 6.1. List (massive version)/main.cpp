#include "list.h"

int main()
{
    LIST_CONSTR(lst, 3);

    int saved_pos = 0;

    saved_pos = insert_after_list(&lst, 1, 10);
    saved_pos = insert_after_list(&lst, saved_pos, 14);
    saved_pos = insert_after_list(&lst, saved_pos, 50);
    saved_pos = insert_after_list(&lst, saved_pos, 40);
    saved_pos = insert_after_list(&lst, saved_pos, 60);
    saved_pos = push_front_list(&lst, 80);
    saved_pos = push_front_list(&lst, 800);

    sort_list(&lst);

    lst.elem[3].prev = 5;

    dump_list(&lst);

    destruct_list(&lst);

    system("pause");

    return 0;
}
