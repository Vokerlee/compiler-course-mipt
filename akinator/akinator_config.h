#ifndef AKINATOR_CONFIG_H_INCLUDED
#define AKINATOR_CONFIG_H_INCLUDED

#define UTF8_CONV

#include <ctype.h>
#include <locale.h>
#include <stdio.h>

#define TX_USE_SPEAK
#include "TXLib.h"

#include "tree.h"
#include "text.h"

const int OFF            = 0;
const int ON             = 1;
const int MAX_CMD_LENGTH = 150;

const char PATH_CODE[]      = {"iconv.exe -t UTF-8 -f CP1251 < graph_base.dot > graph_base_temp.dot"};
const char GRAPH_BASE_ALT[] = {"dot -Tpdf -o graph_base.pdf graph_base.dot"};
const char GRAPH_BASE[]     = {"dot -Tpdf -o graph_base.pdf graph_base_temp.dot"};
const char DELETE_OLD[]     = {"del graph_base.dot"};
const char RENAME_NEW[]     = {"ren graph_base_temp.dot graph_base.dot"};
const char PRINT_BASE[]     = {"start graph_base.pdf"};

#endif // AKINATOR_CONFIG_H_INCLUDED
