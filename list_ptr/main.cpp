#include "p_list.h"

int main()
{
    P_LIST_CONSTR(lst)

    p_linked_list_el *temp = insert_before_p_list(&lst, nullptr, 10);
    temp = insert_before_p_list(&lst, temp, 20);
    temp = insert_before_p_list(&lst, temp, 30);
    temp = insert_before_p_list(&lst, temp, 40);
    temp = insert_after_p_list(&lst, temp, 70);
    push_front_p_list(&lst, 100);

    erase_p_list(&lst, temp);

    printf("%lg\n", find_value_p_list(&lst, 3));

    dump_p_list(&lst);

    destruct_p_list(&lst);

    system("pause");

    return 0;
}
