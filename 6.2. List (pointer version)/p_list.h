#ifndef P_LIST_H_INCLUDED
#define P_LIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#include "p_list_config.h"

#define P_LIST_CONSTR(list)                 \
    p_linked_list list = {};                \
    construct_p_list(&list, #list);

struct p_linked_list_el
{
    double data = 0;

    p_linked_list_el *next = nullptr;

    p_linked_list_el *prev = nullptr;
};

struct p_linked_list
{
    int curr_size = 0;

    p_linked_list_el *list_front = nullptr;

    p_linked_list_el *list_back = nullptr;

    int error_state = 0;

    char *list_name = nullptr;
};

void construct_p_list (p_linked_list *lst, const char *name);

void destruct_p_list (p_linked_list *lst);

p_linked_list_el *insert_before_p_list (p_linked_list *lst, p_linked_list_el *pos, double value);

p_linked_list_el *insert_after_p_list (p_linked_list *lst, p_linked_list_el *pos, double value);

p_linked_list_el *push_front_p_list (p_linked_list *lst, double value);

p_linked_list_el *push_back_p_list (p_linked_list *lst, double value);

void erase_p_list (p_linked_list *lst, p_linked_list_el *pos);

void clear_p_list (p_linked_list *lst);

p_linked_list_el *front_p_list (p_linked_list *lst);

p_linked_list_el *back_p_list (p_linked_list *lst);

p_linked_list_el *pos_after_p_list (p_linked_list *lst, p_linked_list_el *pos);

p_linked_list_el *pos_before_p_list (p_linked_list *lst, p_linked_list_el *pos);

int find_index_p_list (p_linked_list *lst, double value);

double find_value_p_list (p_linked_list *lst, int pos);

const char *error_text_p_list (int error);

int error_p_list (p_linked_list *lst);

void dump_p_list (p_linked_list *lst);


#endif // P_LIST_H_INCLUDED
