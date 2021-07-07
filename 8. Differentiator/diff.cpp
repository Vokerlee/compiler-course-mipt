#include "diff.h"

int ERROR_STATE = 0;

#define SKIP_SPACES                                                                             \
    while (isspace(*(text->counter)))                                                           \
    {                                                                                           \
        if (*(text->counter) == '\n')                                                           \
            text->counter = text->lines[++(text->line_counter)].line;                           \
        else                                                                                    \
            text->counter++;                                                                    \
    }

#define L element->left
#define R element->right
#define dR diff_tree_elem(element->right, tex, var)
#define dL diff_tree_elem(element->left, tex, var)
#define cL copy_tree(element->left)
#define cR copy_tree(element->right)

#define MULTIPLY(left, right)                                                                   \
    create_tree_element(OPER, MUL, left, right)

#define DIVIDE(left, right)                                                                     \
    create_tree_element(OPER, DIV, left, right)

#define ADDITION(left, right)                                                                   \
    create_tree_element(OPER, ADD, left, right)

#define SUBTRACT(left, right)                                                                   \
    create_tree_element(OPER, SUB, left, right)

#define POWER(left, right)                                                                      \
    create_tree_element(OPER, POW, left, right)

#define POWER_NUM(element, value)                                                               \
    create_tree_element(OPER, POW, element, create_tree_element(NUM, value, nullptr, nullptr))

#define CR_NUM(value)                                                                           \
    create_tree_element(NUM, value, nullptr, nullptr)

#define CR_VAR(value)                                                                           \
    create_tree_element(VAR, value, nullptr, nullptr)

#define ORIG_FORMULA_PRINT                                                     \
    fprintf(tex, "\n%s\n"                                                      \
                 "\\begin{equation}\n"                                         \
                 "\\left(", phrase_cond_print());                              \
    print_formula(element, tex, var, BRACKETS_OFF);                            \
    fprintf(tex, " \\right)\'\n"                                               \
                 "\\end{equation}\n"                                           \
                 "%s\n\\begin{equation}\n", phrase_eq_print());

#define DIFF_FUNC_PRINT(command)                                          \
    fprintf(tex, #command " \\left( ");                                   \
    print_formula(L, tex, var, BRACKETS_OFF);                             \
    fprintf(tex, " \\right) \\cdot \\left( ");                            \
    print_formula(L, tex, var, BRACKETS_OFF);                             \
    fprintf(tex, " \\right)\'\n"                                          \
                 "\\end{equation}\n");

#define CREATE_FUNC(name, num)                                            \
{                                                                         \
    vertex = create_tree_element(FUNC, name, nullptr, nullptr);           \
                                                                          \
    text->counter += num;                                                 \
                                                                          \
    vertex->left = create_p_tree(var, elem, text);                        \
}                                                                         \


void create_diff_article (FILE *formula)
{
    assert(formula);

    srand(time(NULL));

    bin_tree tree = {};
    construct_tree(&tree, "tree");

    bin_tree diff_tree = {};
    construct_tree(&diff_tree, "diff_tree");

    variables var = {};
    elements elem = {};

    elem.elements_ = (bin_tree_elem **) calloc(MAX_TREE_ELEM, sizeof(bin_tree_elem *));

    fill_tree(&tree, formula, &var, &elem);

    free(elem.elements_);

    if (tree.root == nullptr)
    {
        destruct_tree(&tree);
        destruct_tree(&diff_tree);

        return;
    }

    optimize_tree(&tree);

    print_diff_tex(&diff_tree, &tree, &var);

    destruct_tree(&tree);
    destruct_tree(&diff_tree);
}

void fill_tree (bin_tree *tree, FILE *formula, variables *var, elements *elem)
{
    assert(formula);
    assert(var);
    assert(elem);

    ASSERT_TREE_OK_VOID

    text_t text = {};
    construct_text(&text);

    fill_text(formula, &text);

    text.counter = text.lines[0].line;

    tree->root = create_e_tree(var, elem, &text);

    if (tree->root == nullptr)
        return;
    else if (*(text.counter) == '\n' || *(text.counter) == '\0')
        text.counter++;
    else
    {
        ERROR_STATE = END_OF_LINE_ERR;
        syntax_error(&text, elem, __LINE__, __FILE__);
        tree->root = nullptr;

        return;
    }

    destruct_text(&text);

    int size_tree = 0;
    recalc_size_tree(tree->root, &size_tree);
    tree->tree_size = size_tree;

    ASSERT_TREE_OK_VOID
}

bin_tree_elem *create_e_tree (variables *var, elements *elem, text_t *text)
{
    bin_tree_elem *op     = nullptr;
    bin_tree_elem *vertex = nullptr;

    int sign = 1;

    SKIP_SPACES

    if (*(text->counter) == '-')
    {
        sign = -1;
        text->counter++;
    }

    vertex = create_t_tree(var, elem, text);
    op     = vertex;

    if (sign == -1)
        vertex = MULTIPLY(CR_NUM(-1), vertex);

    while (*(text->counter) == '+' || *(text->counter) == '-')
    {
        if (*(text->counter) == '+')
            op = create_tree_element(OPER, ADD, nullptr, nullptr);
        else
            op = create_tree_element(OPER, SUB, nullptr, nullptr);

        op->left = vertex;

        text->counter++;

        op->right = create_t_tree(var, elem, text);
        vertex = op;
    }

    SKIP_SPACES

    return vertex;
}

bin_tree_elem *create_t_tree (variables *var, elements *elem, text_t *text)
{
    bin_tree_elem *op     = nullptr;
    bin_tree_elem *vertex = nullptr;

    SKIP_SPACES

    vertex = create_w_tree(var, elem, text);
    op     = vertex;

    if (*(text->counter) == '*' || *(text->counter) == '/')
    {
        while (*(text->counter) == '*' || *(text->counter) == '/')
        {
            if (*(text->counter) == '*')
                op = create_tree_element(OPER, MUL, nullptr, nullptr);
            else
                op = create_tree_element(OPER, DIV, nullptr, nullptr);

            op->left = vertex;

            text->counter++;

            op->right = create_w_tree(var, elem, text);
            vertex = op;
        }
    }

    SKIP_SPACES

    return vertex;
}

bin_tree_elem *create_w_tree (variables *var, elements *elem, text_t *text)
{
    bin_tree_elem **mass     = (bin_tree_elem **) calloc(MAX_OPER_ROW, sizeof(bin_tree_elem *));
    bin_tree_elem **operands = (bin_tree_elem **) calloc(MAX_OPER_ROW, sizeof(bin_tree_elem *));

    SKIP_SPACES

    mass[0] = create_p_tree(var, elem, text);
    bin_tree_elem *vertex = mass[0];

    if (mass[0] == nullptr)
        return nullptr;

    int quant = 1;

    if (*(text->counter) == '^')
    {
        while (*(text->counter) == '^')
        {
            operands[quant - 1] = create_tree_element(OPER, POW, nullptr, nullptr);

            elem->elements_[elem->curr_size_++] = operands[quant - 1];

            text->counter++;

            mass[quant++] = create_p_tree(var, elem, text);

            if (mass[quant - 1] == nullptr)
                return nullptr;
        }

        operands[quant - 2]->left  = mass[quant - 2];
        operands[quant - 2]->right = mass[quant - 1];

        for (int i = quant - 3; i >= 0; i--)
        {
            operands[i]->left  = mass[i];
            operands[i]->right = operands[i + 1];
        }

        vertex = operands[0];
    }

    free(operands);
    free(mass);

    return vertex;
}

bin_tree_elem *create_p_tree (variables *var, elements *elem, text_t *text)
{
    bin_tree_elem *vertex = nullptr;

    SKIP_SPACES

    if (*(text->counter) == '(')
    {
        text->counter++;
        vertex = create_e_tree(var, elem, text);

        if (vertex == nullptr)
            return nullptr;

        if (*(text->counter) == ')')
            text->counter++;
        else
        {
            ERROR_STATE = BRAKETS_ERR;
            syntax_error(text, elem, __LINE__, __FILE__);

            return nullptr;
        }
    }
    else
    {
        vertex = create_n_tree(var, elem, text);

        if (vertex == nullptr)
            return nullptr;
    }

    return vertex;
}

bin_tree_elem *create_n_tree (variables *var, elements *elem, text_t *text)
{
    double value = 0;

    bin_tree_elem *vertex = nullptr;

    SKIP_SPACES

    if (isdigit(*(text->counter)) || (isdigit(*(text->counter + 1)) && *(text->counter) == '-'))
    {
        value = atof(text->counter);
        if (*(text->counter) == '-')
            text->counter++;

        while (isdigit(*(text->counter)) || *(text->counter) == '.')
            text->counter++;

        vertex = create_tree_element(NUM, value, nullptr, nullptr);
    }
    else if (strncmp(text->counter, "sin", 3) == 0 && *(text->counter + 3) == '(')
        CREATE_FUNC(SIN, 3)
    else if (strncmp(text->counter, "cos", 3) == 0 && *(text->counter + 3) == '(')
        CREATE_FUNC(COS, 3)
    else if (strncmp(text->counter, "tg", 2) == 0 && *(text->counter + 2) == '(')
        CREATE_FUNC(TG, 4)
    else if (strncmp(text->counter, "ctg", 3) == 0 && *(text->counter + 3) == '(')
        CREATE_FUNC(CTG, 3)
    else if (strncmp(text->counter, "arcsin", 6) == 0 && *(text->counter + 6) == '(')
        CREATE_FUNC(ARCSIN, 6)
    else if (strncmp(text->counter, "arccos", 6) == 0 && *(text->counter + 6) == '(')
        CREATE_FUNC(ARCCOS, 6)
    else if (strncmp(text->counter, "arctg", 5) == 0 && *(text->counter + 5) == '(')
        CREATE_FUNC(ARCTG, 5)
    else if (strncmp(text->counter, "arcctg", 6) == 0 && *(text->counter + 6) == '(')
        CREATE_FUNC(ARCCTG, 6)
    else if (strncmp(text->counter, "sh", 2) == 0 && *(text->counter + 2) == '(')
        CREATE_FUNC(SH, 2)
    else if (strncmp(text->counter, "ch", 2) == 0 && *(text->counter + 2) == '(')
        CREATE_FUNC(CH, 2)
    else if (strncmp(text->counter, "th", 2) == 0 && *(text->counter + 2) == '(')
        CREATE_FUNC(TH, 2)
    else if (strncmp(text->counter, "cth", 3) == 0 && *(text->counter + 3) == '(')
        CREATE_FUNC(CTH, 3)
    else if (strncmp(text->counter, "ln", 2) == 0 && *(text->counter + 2) == '(')
        CREATE_FUNC(LN, 2)
    else if (isalpha(*(text->counter)) != 0)
    {
        int num_var = var_search(var, text, elem);
        if (num_var == -1)
            return nullptr;

        vertex = create_tree_element(VAR, num_var, nullptr, nullptr);
    }
    else
    {
        ERROR_STATE = UNKNOWN_DESIGN;
        syntax_error(text, elem, __LINE__, __FILE__);

        return nullptr;
    }

    elem->elements_[elem->curr_size_++] = vertex;

    return vertex;
}

int var_search (variables *var, text_t *text, elements *elem)
{
    char *start = text->counter;

    for (int i = 0; i < var->n_var; i++)
    {
        if (strncmp(text->counter, var->var[i], strlen(var->var[i])) == 0)
        {
            text->counter++;
            return i;
        }
    }

    text->counter = start;

    var->n_var++;

    while (isalpha(*(text->counter)) != 0)
        var->var[var->n_var - 1][text->counter++ - start] = *(text->counter);

    while (isspace(*(text->counter)) && *(text->counter) != '\0')
    {
        if (*(text->counter) == '\n')
            text->counter = text->lines[++(text->line_counter)].line;
        else
            text->counter++;
    }

    if (*(text->counter) == '(')
    {
        ERROR_STATE = UNKNOWN_DESIGN;
        text->counter = start;
        syntax_error(text, elem, __LINE__, __FILE__);

        return -1;
    }

    return var->n_var - 1;
}

void print_diff_tex (bin_tree *diff_tree, bin_tree *tree, variables *var)
{
    FILE *tex = fopen("article.tex", "wb");
    assert(tex);

    fprintf(tex, "\\documentclass[a4paper, 12pt]{article}\n"
                 "\\usepackage[T2A]{fontenc}\n"
                 "\\usepackage[utf8]{inputenc}\n"
                 "\\usepackage[english, russian]{babel}\n"
                 "\\usepackage{amsmath, amsfonts, amssymb, amsthm, mathtools, indentfirst}\n\n"
                 "\\usepackage{wasysym}\n"
                 "\\usepackage{lscape}\n"
                 "\\usepackage{xcolor}\n"
                 "\\usepackage{titlesec}\n"
                 "\\titlelabel{ \\thetitle.\\quad }\n"
                 "\\usepackage{hyperref}\n"
                 "\\usepackage[normalem]{ulem}\n\n"
                 "\\hypersetup\n{\n"
                    "\tcolorlinks = true,\n"
                    "\tlinkcolor  = blue,\n"
                    "\tfilecolor  = magenta,\n"
                    "\turlcolor   = cyan,\n"
                 "}\n"

                 "\\usepackage{fancyhdr}\n"
                 "\\pagestyle{fancy}\n"
                 "\\fancyhead{}\n"
                 "\\fancyhead[L]{Как вычислять производную}\n"
                 "\\fancyhead[R]{Глаз Роман, группа Б01-007}\n"
                 "\\fancyfoot[C]{\\thepage}\n"

                 "\\begin{document}\n\n"

                 "\\binoppenalty = 10000\n"
                 "\\relpenalty = 10000\n\n"
                 "\\begin{titlepage}\n"
	             "\\newpage\n\n"
                 "\\begin{center}\n"
		         "\\normalsize Московский физико-технический институт \\\\(госудраственный университет)\n"
	             "\\end{center}\n"
	             "\\vspace{6em}\n"
                 "\\begin{center}\n"
                     "\\Large Информатика\\\\Вопрос по выбору\n"
                 "\\end{center}\n"
                 "\\vspace{1em}\n"
                 "\\begin{center}\n"
                     "\t\\large \\textbf{Лабораторная работа по теории вероятности}\n"
                 "\\end{center}\n"
                 "\\vspace{2em}\n"
                 "\\begin{center}\n"
                     "\t\\large Глаз Роман Сергеевич\\\\Группа Б01-007\n"
                 "\\end{center}\n"
                 "\\vspace{\\fill}\n"
                 "\\begin{center}\n"
                     "\tДолгопрудный \\\\2021\n"
                 "\\end{center}\n"
                 "\\end{titlepage}\n\n"

                 "\\tableofcontents\n"
                 "\\newpage\n\n"

                 "\\section{Как вычислить производную}\n"
                 "Как известно, вероятность того, что вы правильно найд те производную, равна $(\\pi - \\text{e})$ [Неопубликованные исследования Ландау. Часть 3, стр. 1045]. "
                 "Но мы докажем, что эту вероятность можно повысить с помощью этой чудесной статьи, "
                 "также вы научитесь считать арифметические выражения вида $2+2$ и наконец вспомните, что такое правильно брать производные.");

	fprintf(tex, "\nДля разминки вычислим следующую элементарную производную:\n");

    fprintf(tex, "\\begin{equation}\n\t");
    print_formula(tree->root, tex, var, BRACKETS_OFF);
    fprintf(tex, "\n\\end{equation}\n");

    diff_tree->root = diff_tree_elem(tree->root, tex, var);

    int size_tree = 0;
    recalc_size_tree(diff_tree->root, &size_tree);
    diff_tree->tree_size = size_tree;

    optimize_tree(diff_tree);

    fprintf(tex, "\nПолучили результат, который, впринципе, мог быть подсчитан устно:\n");

    if (diff_tree->tree_size >= MIN_SUB_TREE)
    {
        substitutions sub = {};
        sub.capacity = diff_tree->tree_size;
        sub.subst = (subst_elem *) calloc(diff_tree->tree_size, sizeof(subst_elem));

        create_substitutions(diff_tree, &sub);

        size_tree = 0;
        recalc_size_tree(diff_tree->root, &size_tree);
        diff_tree->tree_size = size_tree;

        fprintf(tex, "\\begin{equation}\n"
                     "\\boxed{");
        print_formula(diff_tree->root, tex, var, BRACKETS_OFF);
        fprintf(tex, "}\n\\end{equation}\n");

        fprintf(tex, "\nЗдесь мы для удобства ввели следующие замены:\n");

        for (int i = 0; i < sub.curr_size; i++)
        {
            fprintf(tex, "\\begin{equation}\n"
                         "\t%c = ", i + 'A');
            print_formula(sub.subst[i].subst, tex, var, BRACKETS_OFF);
            fprintf(tex, "\n\\end{equation}\n\n");
        }

        fprintf(tex, "\n\n");

        free(sub.subst);
    }
    else
    {
        fprintf(tex, "\\begin{equation}\n");
        print_formula(diff_tree->root, tex, var, BRACKETS_OFF);
        fprintf(tex, "\\end{equation}\n\n");
    }

    fprintf(tex, "\\section{Список используемой литературы}\n"
		 "При решении задачи мы воспользовались следующими источниками (мы должны отдать дань этим авторам за помощь):\n\n"
		 "[1] Киселев А. П. Систематический курс арифметики [1915]\n\n"
		 "[2] Юшкевич А. М. История Математики (с древнейших времен и до 19 века) в 3-х томах [1970]\n\n"
		 "[3] Ландау Л. Д., Лифшиц Е. М. Теоретическая физика, Статистическая физика, Том 9, Часть 2\n\n"
		 "[4] Рыбников Ю. С. Таблица производных от древних Русов\n\n"
		 "[5] Гениальный автор этого текста, который знает всего 10-15 фраз на всю статью и его "
         "\\href{https://github.com/Vokerlee/Introduction-to-compiler-technologies/tree/master/8.%%20Differentiator}{репозиторий}.\n\n");
    fprintf(tex, "\n\\end{document}\n");

    fclose(tex);

    // This block of code is used only in case, when compiler generates file with CP1251 encoding instead of UTF-8
    //system(PATH_CODE);
    //system(DELETE_OLD);
    //system(RENAME_NEW);
    // In all other cases ignore this code

    system(TEX_CMD);
    system(START_TEX);
}

void print_formula (bin_tree_elem *element, FILE *tex, variables *var, int priority)
{
    assert(tex);

    if (priority == BRACKETS_ON)
        fprintf(tex, "\\left(");

    if (element->type == FUNC)
        print_func_text((int) element->value, tex);

    if (L != nullptr && element->type == OPER && (int) element->value == DIV)
    {
        fprintf(tex, "\\frac{");
        print_formula(L, tex, var, compare_priorities(element, L));
        fprintf(tex, "}{");
    }
    else if (L != nullptr && element->type == OPER && (int) element->value == POW)
    {
        if (L->type == FUNC)
        {
            print_func_text((int) L->value, tex);
            fprintf(tex, "^{");
            print_formula(R, tex, var, BRACKETS_OFF);
            fprintf(tex, "}");
            print_formula(L->left, tex, var, compare_priorities(L, L->left));

            if (priority == BRACKETS_ON)
                fprintf(tex, "\\right)");

            return;
        }

        print_formula(L, tex, var, compare_priorities(element, L));
        fprintf(tex, "^{");
    }
    else if (L != nullptr && element->type != SUBST)
        print_formula(L, tex, var, compare_priorities(element, L));

    switch (element->type)
    {
        case VAR:
        {
            fprintf(tex, "%s", var->var[(int) element->value]);
            break;
        }
        case NUM:
        {
            fprintf(tex, "%lg", element->value);
            break;
        }
        case OPER:
        {
            if ((int) element->value == MUL)
                fprintf(tex, " \\cdot ");
            else if ((int) element->value != DIV && (int) element->value != POW)
                fprintf(tex, " %c ", (char) element->value);

            break;
        }
        case SUBST:
            fprintf(tex, "%c", (int) element->value + 'A');
    }

    if (R != nullptr && element->type == OPER && (int) element->value == DIV)
    {
        print_formula(R, tex, var, compare_priorities(element, R));
        fprintf(tex, "}");
    }
    else if (R != nullptr && element->type == OPER && (int) element->value == POW)
    {
        print_formula(R, tex, var, BRACKETS_OFF);
        fprintf(tex, "}");
    }
    else if (R != nullptr)
        print_formula(R, tex, var, compare_priorities(element, R));

    if (priority == BRACKETS_ON)
        fprintf(tex, "\\right)");
}

int compare_priorities (bin_tree_elem *element1, bin_tree_elem *element2)
{
    assert(element1);
    assert(element2);

    int elem1_type = element1->type;
    int elem2_type = element2->type;

    int elem1_value = (int) element1->value;
    int elem2_value = (int) element2->value;

    if (elem1_type == FUNC)
    {
        if (elem2_type == NUM || elem2_type == VAR)
            return BRACKETS_OFF;

        return BRACKETS_ON;
    }

    if (elem1_type == OPER)
    {
        if (elem1_value == ADD || elem1_value == SUB || elem1_value == DIV)
                return BRACKETS_OFF;

        if (elem2_type == OPER || elem2_type == FUNC)
        {
            if (elem1_value == POW)
                return BRACKETS_ON;

            if (elem1_value == MUL)
            {
                if (elem2_value == ADD || elem2_value == SUB)
                    return BRACKETS_ON;

                if (elem2_value == MUL || elem2_value == DIV || elem2_value == POW || elem2_value == FUNC)
                    return BRACKETS_OFF;
            }
        }

        if (element1->value == DIV && elem2_type == NUM && element2->value < 0)
            return BRACKETS_OFF;

        if (elem2_type == NUM && element2->value < 0)
            return BRACKETS_ON;

        if (elem2_type == NUM)
            return BRACKETS_OFF;

        if (elem2_type == VAR)
            return BRACKETS_OFF;

        if (elem1_value == POW && (elem2_type == NUM || elem2_type == VAR))
            return BRACKETS_OFF;
    }

    return BRACKETS_OFF;
}

bin_tree_elem *create_tree_element (int type, double value, bin_tree_elem *left, bin_tree_elem *right)
{
    bin_tree_elem *element = (bin_tree_elem *) calloc(1, sizeof(bin_tree_elem));

    element->type  = type;
    element->value = value;
    L = left;
    R = right;

    return element;
}

bin_tree_elem *copy_tree_elem (bin_tree_elem *elem2)
{
    bin_tree_elem *elem1 = (bin_tree_elem *) calloc(1, sizeof(bin_tree_elem));

    elem1->type  = elem2->type;
    elem1->value = elem2->value;

    return elem1;
}

bin_tree_elem *copy_tree (bin_tree_elem *element)
{
    bin_tree_elem *new_elem = copy_tree_elem(element);

    if (L != nullptr)
        new_elem->left = copy_tree(L);

    if (R != nullptr)
        new_elem->right = copy_tree(R);

    return new_elem;
}

int is_elem_func (bin_tree_elem *element)
{
    int state1 = 0;
    int state2 = 0;

    if (element->type == VAR)
        return 1;

    if (L != nullptr)
        state1 = is_elem_func(L);

    if (R != nullptr)
        state2 = is_elem_func(R);

    return state1 + state2;
}

bin_tree_elem *diff_tree_elem (bin_tree_elem *element, FILE *tex, variables *var)
{
    switch (element->type)
    {
        case NUM:
        {
            fprintf(tex, "\n%s\n\\begin{equation}\n", phrase_cond_print());
            print_formula(element, tex, var, BRACKETS_OFF);
            fprintf(tex, " \'\\end{equation}\n%s\n\\[0\\]\n", phrase_eq_print());

            return CR_NUM(0);
        }
        case VAR:
            return CR_NUM(1);
        case OPER:
        {
            switch ((int) element->value)
            {
                case ADD:
                {
                    ORIG_FORMULA_PRINT

                    fprintf(tex, "\\left( ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right) \' + ");
                    fprintf(tex, "\\left( ");
                    print_formula(R, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right)\'");
                    fprintf(tex, "\n\\end{equation}\n");

                    return ADDITION(dL, dR);
                }
                case SUB:
                {
                    ORIG_FORMULA_PRINT

                    fprintf(tex, "\\left(");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right)\' - ");
                    fprintf(tex, "\\left( ");
                    print_formula(R, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right)\'");
                    fprintf(tex, "\n\\end{equation}\n");

                    return SUBTRACT(dL, dR);
                }
                case MUL:
                {
                    ORIG_FORMULA_PRINT

                    fprintf(tex, "\\left(");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right)\' \\cdot ");
                    print_formula(R, tex, var, BRACKETS_OFF);
                    fprintf(tex, " + ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, "\\cdot \\left( ");
                    print_formula(R, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right)\'");
                    fprintf(tex, "\n\\end{equation}\n");

                    return ADDITION(MULTIPLY(dL, cR), MULTIPLY(cL, dR));
                }
                case DIV:
                {
                    ORIG_FORMULA_PRINT

                    fprintf(tex, "\\frac{\\left(");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right)\' \\cdot ");
                    print_formula(R, tex, var, BRACKETS_OFF);
                    fprintf(tex, " - ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, "\\cdot \\left(");
                    print_formula(R, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right)\'}{\\left(");
                    print_formula(R, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right)^2}");
                    fprintf(tex, "\n\\end{equation}\n");

                    return DIVIDE(SUBTRACT(MULTIPLY(dL, cR), MULTIPLY(cL, dR)), POWER_NUM(cR, 2));
                }
                case POW:
                {
                    ORIG_FORMULA_PRINT

                    if (L->type == NUM && R->type == NUM)
                    {;
                        fprintf(tex, "0\n\\end{equation}\n");

                        return CR_NUM(0);
                    }
                    else if (L->type == NUM && R->type == VAR)
                    {
                        print_formula(element, tex, var, BRACKETS_OFF);
                        fprintf(tex, "\\cdot ln \\left( %lg \\right)", L->value);
                        fprintf(tex, "\n\\end{equation}\n");

                        return MULTIPLY(copy_tree(element), create_tree_element(FUNC, LN, CR_NUM(L->value), nullptr));
                    }
                    else if (L->type == NUM && (R->type == FUNC || R->type == OPER))
                    {
                        print_formula(element, tex, var, BRACKETS_OFF);
                        fprintf(tex, "\\cdot ln(%lg) \\cdot \\left(", L->value);
                        print_formula(R, tex, var, BRACKETS_OFF);
                        fprintf(tex, " \\right) \'");
                        fprintf(tex, "\n\\end{equation}\n");

                        return MULTIPLY(MULTIPLY(copy_tree(element), create_tree_element(FUNC, LN, CR_NUM(L->value), nullptr)), dR);
                    }
                    else if (L->type == VAR && R->type == NUM)
                    {
                        print_formula(R, tex, var, BRACKETS_OFF);
                        fprintf(tex, "\\cdot \\left(");
                        print_formula(L, tex, var, BRACKETS_OFF);
                        fprintf(tex, " \\right)^%lg", R->value - 1);
                        fprintf(tex, "\n\\end{equation}\n");

                        return MULTIPLY(CR_NUM(R->value), POWER_NUM(CR_VAR(L->value), R->value - 1));
                    }
                    else if (L->type == FUNC && R->type == NUM)
                    {
                        print_formula(R, tex, var, BRACKETS_OFF);
                        fprintf(tex, "\\cdot \\left(");
                        print_formula(L, tex, var, BRACKETS_OFF);
                        fprintf(tex, " \\right)^%lg \\cdot \\left(", R->value - 1);
                        print_formula(L, tex, var, BRACKETS_OFF);
                        fprintf(tex, " \\right) \'");
                        fprintf(tex, "\n\\end{equation}\n");

                        return MULTIPLY(MULTIPLY(CR_NUM(R->value), POWER_NUM(cL, R->value - 1)), dL);
                    }
                    else if (is_elem_func(L) != 0 && is_elem_func(R) != 0)
                    {
                        fprintf(tex, "\\left(e^{");
                        print_formula(R, tex, var, BRACKETS_OFF);
                        fprintf(tex, "\\cdot ln \\left(");
                        print_formula(L, tex, var, BRACKETS_OFF);
                        fprintf(tex, " \\right)}  \\right)\' = e^{");
                        print_formula(R, tex, var, BRACKETS_OFF);
                        fprintf(tex, "\\cdot ln \\left(");
                        print_formula(L, tex, var, BRACKETS_OFF);
                        fprintf(tex, " \\right)} \\cdot \\left( ");
                        print_formula(R, tex, var, BRACKETS_OFF);
                        fprintf(tex, "\\cdot ln \\left(");
                        print_formula(L, tex, var, BRACKETS_OFF);
                        fprintf(tex, " \\right) \\right)\'");
                        fprintf(tex, "\n\\end{equation}\n");

                        return MULTIPLY(copy_tree(element), diff_tree_elem(MULTIPLY(cR, create_tree_element(FUNC, LN, cL, nullptr)), tex ,var));
                    }
                }
            }
        }
        case FUNC:
        {
            ORIG_FORMULA_PRINT

            switch ((int) element->value)
            {
                case SIN:
                {
                    DIFF_FUNC_PRINT(cos)

                    return MULTIPLY(create_tree_element(FUNC, COS, cL, nullptr), dL);
                }
                case COS:
                {
                    DIFF_FUNC_PRINT((-1) \\cdot sin)

                    return MULTIPLY(MULTIPLY(create_tree_element(FUNC, COS, cL, nullptr), CR_NUM(-1)), dL);
                }
                case TG:
                {
                    fprintf(tex, "\\frac{1}{cos^2 \\left(");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right)} \\cdot \\left(");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right)\'");
                    fprintf(tex, "\n\\end{equation}\n");

                    return MULTIPLY(DIVIDE(CR_NUM(1), POWER(create_tree_element(FUNC, COS, cL, nullptr), CR_NUM(2))), dL);
                }
                case CTG:
                {
                    fprintf(tex, "(-1) \\cdot \\frac{1}{sin^2 \\left( ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right)} \\cdot \\left( ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right)\'");
                    fprintf(tex, "\n\\end{equation}\n");

                    return MULTIPLY(CR_NUM(-1), MULTIPLY(create_tree_element(OPER, DIV, CR_NUM(1), create_tree_element(OPER, POW, create_tree_element(FUNC, SIN, cL, nullptr), CR_NUM(2))), dL));
                }
                case ARCSIN:
                {
                    fprintf(tex, "\\frac{1}{\\sqrt{1-");
                    if (L->type != VAR && L->type != NUM)
                        fprintf(tex, "\\left( ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    if (L->type != VAR && L->type != NUM)
                        fprintf(tex, " \\right)");
                    fprintf(tex, "^2}} \\cdot \\left( ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right) \'");
                    fprintf(tex, "\n\\end{equation}\n");

                    return MULTIPLY(DIVIDE(CR_NUM(1), POWER(SUBTRACT(CR_NUM(1), POWER(cL, CR_NUM(2))), CR_NUM(0.5))), cL);
                }
                case ARCCOS:
                {
                    fprintf(tex, "\\(-1) \\cdot frac{1}{\\sqrt{1-");
                    if (L->type != VAR && L->type != NUM)
                        fprintf(tex, "\\left(");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    if (L->type != VAR && L->type != NUM)
                        fprintf(tex, " \\right)");
                    fprintf(tex, "^2}} \\cdot \\left( ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right) \'");
                    fprintf(tex, "\n\\end{equation}\n");

                    return MULTIPLY(CR_NUM(-1), MULTIPLY(DIVIDE(CR_NUM(1), POWER(SUBTRACT(CR_NUM(1), POWER(cL, CR_NUM(2))), CR_NUM(0.5))), cL));
                }
                case ARCTG:
                {
                    fprintf(tex, "\\frac{1}{1+");
                    if (L->type != VAR && L->type != NUM)
                        fprintf(tex, "\\left( ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    if (L->type != VAR && L->type != NUM)
                        fprintf(tex, " \\right)");
                    fprintf(tex, "^2} \\cdot \\left( ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right)\'");
                    fprintf(tex, "\n\\end{equation}\n");

                    return DIVIDE(CR_NUM(1), ADDITION(CR_NUM(1), POWER(cL, CR_NUM(2))));
                }
                case ARCCTG:
                {
                    fprintf(tex, "(-1) \\cdot \\frac{1}{1+");
                    if (L->type != VAR && L->type != NUM)
                        fprintf(tex, "\\left( ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    if (L->type != VAR && L->type != NUM)
                        fprintf(tex, " \\right)");
                    fprintf(tex, "^2} \\cdot \\left( ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right) \'");
                    fprintf(tex, "\n\\end{equation}\n");

                    return MULTIPLY(CR_NUM(-1), DIVIDE(CR_NUM(1), ADDITION(CR_NUM(1), POWER(cL, CR_NUM(2)))));
                }
                case SH:
                {
                    DIFF_FUNC_PRINT(ch)

                    return MULTIPLY(create_tree_element(FUNC, CH, cL, nullptr), dL);
                }
                case CH:
                {
                    DIFF_FUNC_PRINT(ch)

                    return MULTIPLY(create_tree_element(FUNC, SH, cL, nullptr), dL);
                }
                case TH:
                {
                    fprintf(tex, "\\frac{1}{ch^2");
                    if (L->type != VAR && L->type != NUM)
                        fprintf(tex, "\\left( ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    if (L->type != VAR && L->type != NUM)
                        fprintf(tex, " \\right)");
                    fprintf(tex, "} \\cdot \\left( ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, "\\right) \'");
                    fprintf(tex, "\n\\end{equation}\n");

                    return MULTIPLY(DIVIDE(CR_NUM(1), POWER(create_tree_element(FUNC, CH, cL, nullptr), CR_NUM(2))), dL);
                }
                case CTH:
                {
                    fprintf(tex, "\\frac{1}{sh^2");
                    if (L->type != VAR && L->type != NUM)
                        fprintf(tex, "\\left( ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    if (L->type != VAR && L->type != NUM)
                        fprintf(tex, " \\right)");
                    fprintf(tex, "} \\cdot \\left( ");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right) \'");
                    fprintf(tex, "\n\\end{equation}\n");

                    return MULTIPLY(create_tree_element(OPER, DIV, CR_NUM(1), create_tree_element(OPER, POW, create_tree_element(FUNC, SH, cL, nullptr), CR_NUM(2))), dL);
                }
                case LN:
                {
                    fprintf(tex, "\\frac{1}{");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, "} \\cdot \\left(");
                    print_formula(L, tex, var, BRACKETS_OFF);
                    fprintf(tex, " \\right)\'");
                    fprintf(tex, "\n\\end{equation}\n");

                    return MULTIPLY(DIVIDE(CR_NUM(1), cL), dL);
                }
            }
        }
    }
}

int hash_elem_count (bin_tree_elem *element)
{
    element->hash_tree = element->type + (int) element->value;

    if (L != nullptr)
        element->hash_tree += hash_elem_count(L);

    if (R != nullptr)
        element->hash_tree += hash_elem_count(R);

    return element->hash_tree;
}

int size_elem_count (bin_tree_elem *element)
{
    if (L != nullptr)
        element->elem_size += size_elem_count(L);

    if (R != nullptr)
        element->elem_size += size_elem_count(R);

    if (R == nullptr && L == nullptr)
        element->elem_size = 0;

    element->elem_size += 1;

    return element->elem_size;
}

void create_substitutions (bin_tree *tree, substitutions *sub)
{
    hash_elem_count(tree->root);
    size_elem_count(tree->root);

    substitutions_search(tree->root, sub);

    int prev_size  = sub->curr_size;
    sub->curr_size = 0;

    for (int i = 0; i < prev_size; i++)
    {
        if (sub->subst[i].repeat_num >= MIN_SUBST_NUM || (sub->subst[i].subst->elem_size >= MIN_SUB_UNDERTREE && sub->subst[i].subst->elem_size <= MAX_SUB_UNDERTREE))
        {
            sub->subst[sub->curr_size].subst            = sub->subst[i].subst;
            sub->subst[sub->curr_size].subst->elem_size = sub->subst[i].subst->elem_size;
            sub->subst[sub->curr_size++].repeat_num     = sub->subst[i].repeat_num;
        }
    }

    make_substitutions(tree->root, nullptr, sub, VERTEX);

    for (int i = 0; i < sub->curr_size; i++)
        sub->subst[i].repeat_num = 0;

    recalculate_repeat_num_subst(tree->root, nullptr, sub);

    prev_size = sub->curr_size;
    sub->curr_size = 0;

    for (int i = 0; i < prev_size; i++)
    {
        if (sub->subst[i].repeat_num >= MIN_SUBST_NUM || (sub->subst[i].subst->elem_size >= MIN_SUB_UNDERTREE && sub->subst[i].subst->elem_size <= MAX_SUB_UNDERTREE))
        {
            sub->subst[sub->curr_size].subst            = sub->subst[i].subst;
            sub->subst[sub->curr_size].subst->elem_size = sub->subst[i].subst->elem_size;
            sub->subst[sub->curr_size++].repeat_num     = sub->subst[i].repeat_num;
        }
    }

    delete_unuseful_subst(tree->root, nullptr, sub, VERTEX);
}

void make_substitutions (bin_tree_elem *element, bin_tree_elem *prev, substitutions *sub, int link)
{
    if (L != nullptr)
        make_substitutions(L, element, sub, LEFT);

    for (int i = 0; i < sub->curr_size; i++)
    {
        if (element->hash_tree == sub->subst[i].subst->hash_tree)
        {
            bin_tree_elem *subst = create_tree_element(SUBST, i, element, nullptr);

            if (link == LEFT)
                prev->left = subst;
            else
                prev->right = subst;

            return;
        }
    }

    if (R != nullptr)
        make_substitutions(R, element, sub, RIGHT);
}

void recalculate_repeat_num_subst (bin_tree_elem *element, bin_tree_elem *prev, substitutions *sub)
{
    if (element->type == SUBST)
    {
        for (int i = 0; i < sub->curr_size; i++)
        {
            if (element->left->hash_tree == sub->subst[i].subst->hash_tree)
            {
                sub->subst[i].repeat_num++;

                return;
            }
        }

        return;
    }

    if (L != nullptr)
        recalculate_repeat_num_subst(L, element, sub);

    if (R != nullptr)
        recalculate_repeat_num_subst(R, element, sub);
}

void delete_unuseful_subst (bin_tree_elem *element, bin_tree_elem *prev, substitutions *sub, int link)
{
    if (element->type == SUBST)
    {
        for (int i = 0; i < sub->curr_size; i++)
        {
            if (element->left->hash_tree == sub->subst[i].subst->hash_tree)
            {
                element->value = i;

                delete_all_subst(L, element, LEFT);

                return;
            }
        }

        return;
    }

    if (L != nullptr)
        delete_unuseful_subst(L, element, sub, LEFT);

    if (R != nullptr)
        delete_unuseful_subst(R, element, sub, RIGHT);
}

void delete_all_subst (bin_tree_elem *element, bin_tree_elem *prev, int link)
{
    if (L != nullptr)
        delete_all_subst(L, element, LEFT);

    if (R != nullptr)
        delete_all_subst(R, element, RIGHT);

    if (element->type == SUBST && element->left != nullptr)
    {
        if (link == LEFT)
            prev->left = element->left;
        else
            prev->right = element->left;

        free(element);
    }
}

int search_in_substitutions (substitutions *sub, bin_tree_elem *element)
{
    for (int i = 0; i < sub->curr_size; i++)
    {
        if (element->hash_tree == sub->subst[i].subst->hash_tree)
            return i;
    }

    return -1;
}

void substitutions_search (bin_tree_elem *element, substitutions *sub)
{
    if (element->elem_size >= MIN_SUB_UNDERTREE && element->elem_size <= MAX_SUB_UNDERTREE)
    {
        int search_elem = search_in_substitutions(sub, element);

        if (search_elem == -1)
        {
            sub->subst[sub->curr_size].subst = element;
            sub->subst[sub->curr_size++].repeat_num = 1;
        }
        else
            sub->subst[search_elem].repeat_num++;

        return;
    }

    if (element->elem_size >= MIN_SUBST_SIZE)
    {
        int search_elem = search_in_substitutions(sub, element);

        if (search_elem == -1)
        {
            sub->subst[sub->curr_size].subst = element;
            sub->subst[sub->curr_size++].repeat_num = 1;
        }
        else
            sub->subst[search_elem].repeat_num++;
    }
    else
        return;

    if (L != nullptr)
        substitutions_search(L, sub);

    if (R != nullptr)
        substitutions_search(R, sub);
}

void print_func_text (int value, FILE *tex)
{
    assert(tex);

    switch(value)
    {
        case SIN:
        {
            fprintf(tex, "sin");
            break;
        }
        case COS:
        {
            fprintf(tex, "cos");
            break;
        }
        case TG:
        {
            fprintf(tex, "tg");
            break;
        }
        case CTG:
        {
            fprintf(tex, "ctg");
            break;
        }
        case ARCSIN:
        {
            fprintf(tex, "arcsin");
            break;
        }
        case ARCCOS:
        {
            fprintf(tex, "arccos");
            break;
        }
        case ARCTG:
        {
            fprintf(tex, "arctg");
            break;
        }
        case ARCCTG:
        {
            fprintf(tex, "arcctg");
            break;
        }
        case SH:
        {
            fprintf(tex, "sh");
            break;
        }
        case CH:
        {
            fprintf(tex, "ch");
            break;
        }
        case TH:
        {
            fprintf(tex, "th");
            break;
        }
        case CTH:
        {
            fprintf(tex, "cth");
            break;
        }
        case LN :
        {
            fprintf(tex, "ln");
            break;
        }
    }
}

const char *phrase_cond_print (void)
{
    int random = rand() % 16;

    switch (random)
    {
        case 0:
	        return "Очевидно, что";
	    case 1:
	        return "Любой школьник знает, что";
	    case 2:
	        return "В 4-ом классе вы проходили, что";
	    case 3:
	        return "Ну разве это не очевидно?:";
	    case 4:
	        return "Если не понятно, то просто вот посмотрите и сразу станет понятно, что";
	    case 5:
	        return "Доверьтесь мне, что";
	    case 6:
	        return "Если вы дочитали до этого момента, то поздравляю, вы совсем тупой и не смогли сами вычислить производную!";
	    case 7:
	        return "А это вы найд те в учебнике Кудрявцева [т.1, стр. 127, 7 строка]:";
	    case 8:
	        return "А теперь выполняем следующее преобразование.....:";
	    case 9:
	        return "Лол кек \\sout{чебурек} производная:";
	    case 10:
	        return "Just trust me, that:";
	    case 11:
	        return "Тебе ещ ряды ботать, поэтому давай быстрее:";
	    case 12:
	        return "Если ты дош л до этого момента, значит ты заинтересован и пойм шь следующее:";
	    case 13:
	        return "Тяжко в этом мире...:";
	    case 14:
	        return "Я что, зря писал мой \\href{https://drive.google.com/drive/folders/1cQOob8lLqhboZnilkguOm7CtLHlbPb1G?usp=sharing}{ВПВ по физике}:";
	    default:
	        return "Пока вы это читаете, зацените мой \\href{https://drive.google.com/drive/folders/1cQOob8lLqhboZnilkguOm7CtLHlbPb1G?usp=sharing}{ВПВ по физике}:";
    }
}

const char *phrase_eq_print (void)
{
    int random = rand() % 14;

    switch (random)
    {
        case 0:
	        return "равно";
	    case 1:
	        return "равно (хотя хз)";
	    case 2:
	        return "можно представить в виде";
	    case 3:
	        return "преобразовывается в";
	    case 4:
	        return "слишком сложная производная. Но, согласно таблице, можно получить";
	    case 5:
	        return "здесь должна быть эпичная фраза, но я её не придумал:";
	    case 6:
	        return "с лёгкостью переходит в";
	    case 7:
	        return "с маленькой лёгкостью переходит в";
	    case 8:
	        return "можно преобразовать так:";
	    case 9:
	        return "можно разложить в Ряд, затем проинтегрировать каждое слагамое и взять вторую производную:";
	    case 10:
	        return "более простая форма записи, чем";
	    case 11:
	        return "четыре буквы этой фразы набрала Полторашка:";
	    case 12:
	        return "если вы уже видели ссылку на мой ВПВ, то посмотрите его пожалуйста, если ещё этого не сделали:";
	    default:
	        return "если ты не гомосек, скинь сюда 50Р за 120 сек: +79254492748";
    }
}

void syntax_error (text_t *text, elements *elem, int line, const char *file)
{
    assert(elem);
    printf("Syntax error [%d line in cpp] (%s).\n"
           "Process of file-reading terminated. Please, use the right syntax.\n", line, file);
    printf("You have an error in line %d in file: ", (int) text->lines[text->line_counter].real_num_line + 1);

    switch(ERROR_STATE)
    {
        case BRAKETS_ERR:
        {
            printf("the bracket was missed here: ");
            break;
        }
        case UNKNOWN_DESIGN:
        {
            printf("you have entered unknown command: ");
            break;
        }
        case END_OF_LINE_ERR:
        {
            printf("there is no end of line here: ");
            break;
        }
        default:
        {
            printf("the error is not recognized: ");
            break;
        }
    }

    printf("...");

    for (int i = 0; i < MAX_ERROR_TEXT_LENGTH && *(text->counter) != '\0'; text->counter++, i++)
        printf("%c", *(text->counter));

    printf("...\n\n");

    for (int i = 0; i < elem->curr_size_; i++)
        free(elem->elements_[i]);
}

