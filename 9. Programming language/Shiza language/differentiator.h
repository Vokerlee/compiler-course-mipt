#ifndef DIFFERENTIATE_H_INCLUDED
#define DIFFERENTIATE_H_INCLUDED

#include "tree.h"
#include <ctype.h>
#include <time.h>

const char TEX_CMD[]    = {"pdflatex C:\\CodeBlocks\\Projects\\differentiator\\bin\\Debug\\article.tex"};
const char START_TEX[]  = {"start C:\\CodeBlocks\\Projects\\differentiator\\bin\\Debug\\article.pdf"};
const char PATH_CODE[]  = {"C:\\CodeBlocks\\CodeBlocks\\Config\\iconv.exe -t UTF-8 -f  CP1251 < C:\\CodeBlocks\\Projects\\differentiator\\bin\\Debug\\article.tex > C:\\CodeBlocks\\Projects\\differentiator\\bin\\Debug\\article_temp.tex"};
const char DELETE_OLD[] = {"del C:\\CodeBlocks\\Projects\\differentiator\\bin\\Debug\\article.tex"};
const char RENAME_NEW[] = {"ren C:\\CodeBlocks\\Projects\\differentiator\\bin\\Debug\\article_temp.tex article.tex"};

const int NO_VAR = -1;

const int BRACKETS_ON  = 1;
const int BRACKETS_OFF = 0;

struct elements;
struct variables;


void print_func_text (int value, FILE *tex);

int is_elem_func (bin_tree_elem *element);

bin_tree_elem *derivate_tree (bin_tree_elem *element, int var_value);

int hash_elem_count (bin_tree_elem *element);

int size_elem_count (bin_tree_elem *element);

#endif // DIFFERENTIATE_H_INCLUDED
