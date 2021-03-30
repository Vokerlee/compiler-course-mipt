#include "language_parsing.h"

#define SKIP_SPACES                                                                             \
    while (isspace(*(text->counter)) && *(text->counter) != '\0')                               \
    {                                                                                           \
        if (*(text->counter) == '\n' && text->line_counter < text->n_lines - 1)                 \
            text->counter = text->lines[++(text->line_counter)].line;                           \
        else                                                                                    \
            text->counter++;                                                                    \
    }

#define CREATE_TREE_ELEM(type, value, shift)                                                     \
    {                                                                                            \
        element = create_tree_element(type, value, nullptr, nullptr);                            \
        text->counter += shift;                                                                  \
    }


void tree_to_lang (FILE *tree_lang)
{
    assert(tree_lang);

    FILE *lang = fopen("lang.txt", "wb");

    text_t text = {};
    construct_text(&text);

    fill_text(tree_lang, &text);

    text.counter = text.lines[0].line;

    while ((isspace(*(text.counter)) || *(text.counter) == '{') && *(text.counter) != '\0')
    {
        if (*(text.counter) == '\n' && text.line_counter < text.n_lines - 1)
            text.counter = text.lines[++(text.line_counter)].line;
        else
            text.counter++;
    }

    bin_tree tree = {};
    construct_tree(&tree, "tree");

    variables var = {};

    tree.root = fill_tree(&text, &var);

    lang_transfer(tree.root, lang, &var);

    destruct_text(&text);
    destruct_tree(&tree);

    fclose(lang);
}

int var_search (variables *var, char *temp_var_name)
{
    char *start = temp_var_name;

    for (int i = 0; i < var->curr_size; i++)
    {
        if (strcmp(temp_var_name, var->var[i]) == 0)
        {
            temp_var_name++;
            return i;
        }
    }

    temp_var_name = start;

    var->curr_size++;

    strcpy(var->var[var->curr_size - 1], temp_var_name);

    return var->curr_size - 1;
}

bin_tree_elem *fill_tree (text_t *text, variables *var)
{
    bin_tree_elem *element = nullptr;

    while (isspace(*(text->counter)) && *(text->counter) != '\0')
    {
        if (*(text->counter) == '\n' && text->line_counter < text->n_lines - 1)
            text->counter = text->lines[++(text->line_counter)].line;
        else
            text->counter++;
    }

    if (strncmp(text->counter, "nil", 3) == 0 && !isalpha(*(text->counter + 3)))
    {
        element = nullptr;
        text->counter += 4;
        return element;
    }
    else if (strncmp(text->counter, "concatenation", 13) == 0 && !isalpha(*(text->counter + 13)))
        CREATE_TREE_ELEM(BUNCH, 0, 13)
    else if (strncmp(text->counter, "+", 1) == 0)
        CREATE_TREE_ELEM(OPER, ADD, 1)
    else if (strncmp(text->counter, "-", 1) == 0 && !isdigit(*(text->counter +1)))
        CREATE_TREE_ELEM(OPER, SUB, 1)
    else if (strncmp(text->counter, "*", 1) == 0)
        CREATE_TREE_ELEM(OPER, MUL, 1)
    else if (strncmp(text->counter, "/", 1) == 0)
        CREATE_TREE_ELEM(OPER, DIV, 1)
    else if (strncmp(text->counter, "^", 1) == 0)
        CREATE_TREE_ELEM(OPER, POW, 1)
    else if (strncmp(text->counter, "$sin", 4) == 0 && !isalpha(*(text->counter + 4)))
        CREATE_TREE_ELEM(FUNC, SIN, 4)
    else if (strncmp(text->counter, "$cos", 4) == 0 && !isalpha(*(text->counter + 4)))
        CREATE_TREE_ELEM(FUNC, COS, 4)
    else if (strncmp(text->counter, "$tg", 3) == 0 && !isalpha(*(text->counter + 3)))
        CREATE_TREE_ELEM(FUNC, TG, 3)
    else if (strncmp(text->counter, "$arcsin", 7) == 0 && !isalpha(*(text->counter + 7)))
        CREATE_TREE_ELEM(FUNC, ARCSIN, 7)
    else if (strncmp(text->counter, "$arccos", 7) == 0 && !isalpha(*(text->counter + 7)))
        CREATE_TREE_ELEM(FUNC, ARCCOS, 7)
    else if (strncmp(text->counter, "$arctg", 6) == 0 && !isalpha(*(text->counter + 6)))
        CREATE_TREE_ELEM(FUNC, ARCTG, 6)
    else if (strncmp(text->counter, "$ctg", 4) == 0 && !isalpha(*(text->counter + 4)))
        CREATE_TREE_ELEM(FUNC, CTG, 4)
    else if (strncmp(text->counter, "$scan", 5) == 0 && !isalpha(*(text->counter + 5)))
        CREATE_TREE_ELEM(FUNC, SCAN, 5)
    else if (strncmp(text->counter, "$print", 6) == 0 && !isalpha(*(text->counter + 6)))
        CREATE_TREE_ELEM(FUNC, PRINT, 6)
    else if (strncmp(text->counter, "$diff", 5) == 0 && !isalpha(*(text->counter + 5)))
        CREATE_TREE_ELEM(FUNC, DIFF, 5)
    else if (strncmp(text->counter, "$ln", 3) == 0 && !isalpha(*(text->counter + 3)))
        CREATE_TREE_ELEM(FUNC, LN, 3)
    else if (strncmp(text->counter, "$power", 6) == 0 && !isalpha(*(text->counter + 6)))
        CREATE_TREE_ELEM(FUNC, POWER, 6)
    else if (strncmp(text->counter, "if", 2) == 0 && !isalpha(*(text->counter + 2)))
        CREATE_TREE_ELEM(COMMAND, IF, 2)
    else if (strncmp(text->counter, "while", 5) == 0 && !isalpha(*(text->counter + 5)))
        CREATE_TREE_ELEM(COMMAND, WHILE, 5)
    else if (strncmp(text->counter, "!=", 2) == 0)
       CREATE_TREE_ELEM(CONDITION, JNE, 2)
    else if (strncmp(text->counter, ">=", 2) == 0)
        CREATE_TREE_ELEM(CONDITION, JAE, 2)
    else if (strncmp(text->counter, "<=", 2) == 0)
        CREATE_TREE_ELEM(CONDITION, JBE, 2)
    else if (strncmp(text->counter, ">", 1) == 0)
        CREATE_TREE_ELEM(CONDITION, JA, 1)
    else if (strncmp(text->counter, "<", 1) == 0)
        CREATE_TREE_ELEM(CONDITION, JB, 1)
    else if (strncmp(text->counter, "==", 2) == 0)
        CREATE_TREE_ELEM(CONDITION, JE, 2)
    else if (strncmp(text->counter, "=", 1) == 0)
        CREATE_TREE_ELEM(COMMAND, ASSIGN, 1)
    else if (strncmp(text->counter, "function-declaration", strlen("function-declaration")) == 0)
        CREATE_TREE_ELEM(BUNCH, 0, strlen("function-declaration"))
    else if (strncmp(text->counter, "main", 4) == 0 && !isalpha(*(text->counter + 4)))
         CREATE_TREE_ELEM(MAIN, 0, 4)
    else if (strncmp(text->counter, "return", 6) == 0 && !isalpha(*(text->counter + 6)))
         CREATE_TREE_ELEM(RETURN, 0, 6)
    else if ((*(text->counter) == '$' && isalpha(*(text->counter + 1))) || isalpha(*(text->counter)))
    {
        int type = VAR;

        if (*(text->counter) == '$')
        {
            type = USER_FUNC;
            text->counter++;
        }

        char *temp_var_name = (char *) calloc(MAX_VAR_NAME_LENGTH + 1, sizeof(char));
        char *start = text->counter;
        int num_var = 0;

        if (isalpha(*(text->counter)))
        {
            temp_var_name[text->counter - start] = *(text->counter);
            text->counter++;

            while (isalnum(*(text->counter)) || *(text->counter) == '_')
            {
                temp_var_name[text->counter - start] = *(text->counter);
                text->counter++;
            }
        }

        int var_value = var_search(var, temp_var_name);

        element = create_tree_element(type, var_value, nullptr, nullptr);
    }
    else if (isdigit(*(text->counter)) || (*(text->counter) == '-' && isdigit(*(text->counter + 1))))
    {
        double value = atof(text->counter);

        while (isdigit(*(text->counter)) || *(text->counter) == '.' || *(text->counter) == '-')
            text->counter++;

        element = create_tree_element(NUM, value, nullptr, nullptr);
    }

    SKIP_SPACES

    if (*(text->counter) == '{')
    {
        text->counter++;
        element->left = fill_tree(text, var);
    }

    SKIP_SPACES

    text->counter++;

    SKIP_SPACES

    if (*(text->counter) == '{')
    {
        text->counter++;
        element->right = fill_tree(text, var);
    }

    SKIP_SPACES

    text->counter++;

    SKIP_SPACES

    return element;
}

void print_tab (int *n_tab, FILE *lang)
{
    for (int i = 0; i < *n_tab; i++)
        fprintf(lang, "\t");
}

void lang_transfer (bin_tree_elem *element, FILE *lang, variables *var)
{
    int n_tab = 0;

    for (bin_tree_elem *bunch = element; bunch != nullptr; bunch = bunch->right)
    {
        if (bunch->left->type == COMMAND && bunch->left->value == ASSIGN)
        {
            fprintf(lang, "%s = ", var->var[(int) bunch->left->left->value]);
            print_expression(bunch->left->right, lang, var);
            fprintf(lang, ";\n");
        }
    }

    for (bin_tree_elem *bunch = element; bunch != nullptr; bunch = bunch->right)
    {
        if (bunch->left->type == MAIN)
        {
            fprintf(lang, "\nmain\n{\n");
            n_tab++;
            print_body(bunch->left->right, lang, var, &n_tab);
            n_tab--;
            fprintf(lang, "}\n");
        }
    }

    for (bin_tree_elem *bunch = element; bunch != nullptr; bunch = bunch->right)
    {
        if (bunch->left->type == VAR)
        {
            fprintf(lang, "\n%s(", var->var[(int) bunch->left->value]);

            for (bin_tree_elem *temp_bunch = bunch->left->left; temp_bunch != nullptr; temp_bunch = temp_bunch->left)
            {
                fprintf(lang, "%s", var->var[(int) temp_bunch->value]);

                if (temp_bunch->left != nullptr)
                {
                    fprintf(lang, ", ");
                }
            }

            fprintf(lang, ")\n{\n");
            n_tab++;
            print_body(bunch->left->right, lang, var, &n_tab);
            n_tab--;
            fprintf(lang, "}\n\n");
        }
    }
}

void print_body (bin_tree_elem *element, FILE *lang, variables *var, int *n_tab)
{
    for (bin_tree_elem *bunch = element; bunch != nullptr; bunch = bunch->right)
    {
        if (bunch->left->type == COMMAND && (int) bunch->left->value == ASSIGN)
        {
            print_tab(n_tab, lang);
            fprintf(lang, "%s = ", var->var[(int) bunch->left->left->value]);
            print_expression(bunch->left->right, lang, var);
            fprintf(lang, ";\n");
        }

        if (bunch->left->type == COMMAND && ((int) bunch->left->value == IF || (int) bunch->left->value == WHILE))
        {
            print_tab(n_tab, lang);

            if (bunch->left->value == IF)
                fprintf(lang, "if (");
            else
                fprintf(lang, "while (");

            print_expression(bunch->left->left->left, lang, var);

            switch ((int) bunch->left->left->value)
            {
                case JE:
                {
                    fprintf(lang, " == ");
                    break;
                }
                case JNE:
                {
                    fprintf(lang, " != ");
                    break;
                }
                case JA:
                {
                    fprintf(lang, " > ");
                    break;
                }
                case JAE:
                {
                    fprintf(lang, " >= ");
                    break;
                }
                case JB:
                {
                    fprintf(lang, " < ");
                    break;
                }
                case JBE:
                {
                    fprintf(lang, " <= ");
                    break;
                }
            }

            print_expression(bunch->left->left->right, lang, var);

            fprintf(lang, ")\n");
            print_tab(n_tab, lang);
            fprintf(lang, "{\n");

            (*n_tab)++;
            print_body(bunch->left->right, lang, var, n_tab);
            (*n_tab)--;

            print_tab(n_tab, lang);
            fprintf(lang, "}\n\n");
        }

        if (bunch->left->type == FUNC && (int) bunch->left->value == PRINT)
        {
            print_tab(n_tab, lang);
            fprintf(lang, "print(");
            print_expression(bunch->left->left, lang, var);
            fprintf(lang, ");\n");
        }

        if (bunch->left->type == FUNC && (int) bunch->left->value == SCAN)
        {
            print_tab(n_tab, lang);
            fprintf(lang, "scan(%s);\n", var->var[(int) bunch->left->left->value]);
        }

        if (bunch->left->type == RETURN)
        {
            print_tab(n_tab, lang);
            fprintf(lang, "return");

            if (bunch->left->left != nullptr)
            {
                fprintf(lang, " ");
                print_expression(bunch->left->left, lang, var);
            }

            fprintf(lang, ";\n");
        }

        if (bunch->left->type == USER_FUNC)
        {
            print_tab(n_tab, lang);
            fprintf(lang, "%s(", var->var[(int) bunch->left->value]);

            for (bin_tree_elem *temp_bunch = bunch->left->left; temp_bunch != nullptr; temp_bunch = temp_bunch->left)
            {
                print_expression(temp_bunch->right, lang, var);

                if (temp_bunch->left != nullptr)
                {
                    fprintf(lang, ", ");
                }
            }

            fprintf(lang, ");\n");
        }

    }
}

void print_expression (bin_tree_elem *element, FILE *lang, variables *var)
{
    if (element->type == NUM)
        fprintf(lang, "%lg", element->value);
    else if (element->type == VAR)
        fprintf(lang, "%s", var->var[(int) element->value]);
    else if (element->type == USER_FUNC)
    {
        fprintf(lang, "%s(", var->var[(int) element->value]);

        for (bin_tree_elem *temp_bunch = element->left; temp_bunch != nullptr; temp_bunch = temp_bunch->left)
        {
            print_expression(temp_bunch->right, lang, var);

            if (temp_bunch->left != nullptr)
            {
                fprintf(lang, ", ");
            }
        }

        fprintf(lang, ")");
    }
    else if (element->type == FUNC)
    {
        switch((int) element->value)
        {
            case SIN:
            {
                fprintf(lang, "sin(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case COS:
            {
                fprintf(lang, "cos(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case ARCSIN:
            {
                fprintf(lang, "arcsin(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case ARCCOS:
            {
                fprintf(lang, "arccos(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case TG:
            {
                fprintf(lang, "tg(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case ARCTG:
            {
                fprintf(lang, "arctg(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case ARCCTG:
            {
                fprintf(lang, "arcctg(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case CTG:
            {
                fprintf(lang, "ctg(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case LN:
            {
                fprintf(lang, "ln(");
                print_expression(element->left, lang, var);
                fprintf(lang, ")");
                break;
            }
            case DIFF:
            {
                fprintf(lang, "diff(%s, ", var->var[(int) element->left->value]);
                print_expression(element->right, lang, var);
                fprintf(lang, ")");
                break;
            }
            case POWER:
            {
                fprintf(lang, "power(");
                print_expression(element->left, lang, var);
                fprintf(lang, ", ");
                print_expression(element->right, lang, var);
                fprintf(lang, ")");
                break;
            }
        }
    }
    else if (element->type == OPER)
    {
        switch ((int) element->value)
        {
            case ADD:
            {
                print_expression(element->left, lang, var);
                fprintf(lang, " + ");
                print_expression(element->right, lang, var);
                break;
            }
            case SUB:
            {
                print_expression(element->left, lang, var);
                fprintf(lang, " - ");
                print_expression(element->right, lang, var);
                break;
            }
            case MUL:
            {
                if (element->left->type != VAR && element->left->type != NUM)
                     fprintf(lang, "(");

                print_expression(element->left, lang, var);

                if (element->left->type != VAR && element->left->type != NUM)
                     fprintf(lang, ")");

                fprintf(lang, " * ");

                if (element->right->type != VAR && element->right->type != NUM)
                     fprintf(lang, "(");

                print_expression(element->right, lang, var);

                if (element->right->type != VAR && element->right->type != NUM)
                     fprintf(lang, ")");
                break;
            }
            case DIV:
            {
                if (element->left->type != VAR && element->left->type != NUM)
                     fprintf(lang, "(");

                print_expression(element->left, lang, var);

                if (element->left->type != VAR && element->left->type != NUM)
                     fprintf(lang, ")");

                fprintf(lang, " / ");

                if (element->right->type != VAR && element->right->type != NUM)
                     fprintf(lang, "(");

                print_expression(element->right, lang, var);

                if (element->right->type != VAR && element->right->type != NUM)
                     fprintf(lang, ")");
                break;
                break;
            }
            case POW:
            {
                if (element->left->type != VAR && element->left->type != NUM)
                     fprintf(lang, "(");

                print_expression(element->left, lang, var);

                if (element->left->type != VAR && element->left->type != NUM)
                     fprintf(lang, ")");

                fprintf(lang, "^");

                if (element->right->type != VAR && element->right->type != NUM)
                     fprintf(lang, "(");

                print_expression(element->right, lang, var);

                if (element->right->type != VAR && element->right->type != NUM)
                     fprintf(lang, ")");
                break;
            }
        }
    }
}
