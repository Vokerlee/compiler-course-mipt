#ifndef LANGUAGE_PARSING_H_INCLUDED
#define LANGUAGE_PARSING_H_INCLUDED

#include "tree.h"
#include <text.h>
#include "language_parsing_config.h"

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

void tree_to_lang (FILE *tree_lang);

int var_search (variables *var, char *temp_var_name);

bin_tree_elem *fill_tree (text_t *text, variables *var);

void print_tab (int *n_tab, FILE *lang);

void lang_transfer (bin_tree_elem *element, FILE *lang, variables *var);

void print_body (bin_tree_elem *element, FILE *lang, variables *var, int *n_tab);

void print_expression (bin_tree_elem *element, FILE *lang, variables *var);

#endif // LANGUAGE_PARSING_H_INCLUDED
