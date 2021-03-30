#ifndef DIFF_CONFIG_H_INCLUDED
#define DIFF_CONFIG_H_INCLUDED

const char TEX_CMD[] = {"pdflatex article.tex"};
const char START_TEX[] = {"start article.pdf"};
const char PATH_CODE[] = {"iconv.exe -t UTF-8 -f CP1251 < article.tex > article_temp.tex"};
const char DELETE_OLD[] = {"del article.tex"};
const char RENAME_NEW[] = {"ren article_temp.tex article.tex"};

const int MAX_VAR_NUM           = 10;
const int MAX_OPER_ROW          = 20;
const int MAX_VAR_NAME_LENGTH   = 20;
const int MAX_ERROR_TEXT_LENGTH = 10;

const int MIN_SUBST_SIZE    = 3;
const int MIN_SUBST_NUM     = 2;
const int MIN_SUB_UNDERTREE = 10;
const int MAX_SUB_UNDERTREE = 38;
const int MIN_SUB_TREE      = 50;

const int NO_VAR = -1;

const int BRACKETS_ON  = 1;
const int BRACKETS_OFF = 0;

const int LEFT         = 0;
const int RIGHT        = 1;
const int VERTEX       = -1;

enum operators
{
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
    POW = '^',
};

enum functions
{
    SIN    = 1,
    COS    = 2,
    TG     = 3,
    CTG    = 4,
    ARCSIN = 5,
    ARCCOS = 6,
    ARCTG  = 7,
    ARCCTG = 8,
    SH     = 9,
    CH     = 10,
    TH     = 11,
    CTH    = 12,
    LN     = 13,
};

enum type
{
    VAR   = 1,
    NUM   = 2,
    OPER  = 3,
    FUNC  = 4,
    SUBST = 5,
};

enum error_state
{
    BRAKETS_ERR     = 1,
    UNKNOWN_DESIGN  = 2,
    END_OF_LINE_ERR = 3,
};

#endif // DIFF_CONFIG_H_INCLUDED
