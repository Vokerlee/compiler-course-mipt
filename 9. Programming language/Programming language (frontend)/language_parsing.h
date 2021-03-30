#ifndef LANGUAGE_PARSING_H_INCLUDED
#define LANGUAGE_PARSING_H_INCLUDED

#include "tree.h"
#include "language_parsing_config.h"

#include <text.h>

#include <algorithm>

struct variables
{
    int curr_size = 0;

    char var[MAX_VAR_NUM][MAX_VAR_NAME_LENGTH] = {0};
};

struct elements
{
    bin_tree_elem **elements = nullptr;

    int curr_size = 0;
};

int var_search (variables *var, char *temp_var_name);

void tree_transfer (bin_tree *tree, variables *var);

int is_keyword (char *temp_var_name, int *type);

bin_tree_elem *create_main_tree (elements *elem, int *counter);

bin_tree_elem *create_body_tree (elements *elem, int *counter);

bin_tree_elem *create_cmd_tree (elements *elem, int *counter);

bin_tree_elem *create_user_func_tree (elements *elem, int *counter);

bin_tree_elem *create_var_func_tree (elements *elem, int *counter);

bin_tree_elem *create_prog_tree (elements *elem, int *counter);

bin_tree_elem *create_n_tree (elements *elem, int *counter);

bin_tree_elem *read_w_tree (elements *elem, int *counter);

bin_tree_elem *create_w_tree (elements *elem, int *counter);

bin_tree_elem *create_t_tree (elements *elem, int *counter);

bin_tree_elem *create_p_tree (elements *elem, int *counter);

bin_tree_elem *create_e_tree (elements *elem, int *counter);

void user_func_optimize (bin_tree_elem *element, int *param, int n_param);

void asm_trans (bin_tree *tree, FILE *assmbl, variables *var);

void func_asm (bin_tree_elem *vertex, FILE *assmbl);

void body_asm (bin_tree_elem *vertex, FILE *assmbl, variables *var);

void command_asm_tree (bin_tree_elem *element, FILE *assmbl, variables *var);

void analyse_expr (bin_tree_elem *element, FILE *assmbl, variables *var);

void lang_asm (FILE *formula);

void fill_tree (bin_tree *tree, FILE *formula, variables *var, elements *elem);

void write_in_tree_file (bin_tree_elem *element, FILE *lang_tree, variables *var);

const char *func_name (int value);

const char *condition_name (int value);

void syntax_error(text_t *text, elements *elem, int line, const char *file);

#endif // LANGUAGE_PARSING_H_INCLUDED
