#include "lang_parsing.h"

int ERROR_STATE = 0;

#define SKIP_SPACES                                                                             \
    while (isspace(*(text.counter)) && *(text.counter) != '\0')                                 \
    {                                                                                           \
        if (*(text.counter) == '\n' && text.line_counter < text.n_lines - 1)                    \
            text.counter = text.lines[++(text.line_counter)].line;                              \
        else                                                                                    \
            text.counter++;                                                                     \
    }

void fill_tree (bin_tree *tree, FILE *formula, variables *var, elements *elem)
{
    assert(formula);
    ASSERT_TREE_OK_VOID

    text_t text = {};
    construct_text(&text);

    fill_text(formula, &text, NO_COMMENTS);

    text.counter = text.lines[0].line;

    double value = 0;

    while (*(text.counter) != '\0')
    {
        printf("%d %c\n", text.lines[text.line_counter].real_num_line, *(text.counter));

        if (isdigit(*(text.counter)))
        {
            value = atof(text.counter);

            while (isdigit(*(text.counter)) || *(text.counter) == '.' || *(text.counter) == '-')
                text.counter++;

            elem->elements[elem->curr_size++] = create_tree_element(NUM, value, nullptr, nullptr);
        }
        else if (isalpha(*(text.counter)) || *(text.counter) == '_')
        {
            char *temp_var_name = (char *) calloc(MAX_VAR_NAME_LENGTH + 1, sizeof(char));
            char *start = text.counter;
            int num_var = 0;

            if (isalpha(*(text.counter)) || *(text.counter) == '_')
            {
                temp_var_name[text.counter - start] = *(text.counter);
                text.counter++;

                while (isalnum(*(text.counter)) || *(text.counter) == '_')
                {
                    temp_var_name[text.counter - start] = *(text.counter);
                    text.counter++;
                }
            }

            int type = 0;
            int op_value = is_keyword(temp_var_name, &type);

            if (op_value == -1)
            {
                num_var = var_search(var, temp_var_name);
                elem->elements[elem->curr_size++] = create_tree_element(VAR, num_var, nullptr, nullptr);
            }
            else
            {
                switch (type)
                {
                    case NUM:
                    {
                        elem->elements[elem->curr_size++] = create_tree_element(NUM, 0, nullptr, nullptr);
                        break;
                    }
                    case FUNC:
                    {
                        elem->elements[elem->curr_size++] = create_tree_element(FUNC, op_value, nullptr, nullptr);
                        break;
                    }
                    case MAIN:
                    {
                        elem->elements[elem->curr_size++] = create_tree_element(MAIN, 0, nullptr, nullptr);
                        break;
                    }
                    case COMMAND:
                    {
                        if (op_value == ASSIGN)
                        {
                            if (text.lines[text.line_counter].length - 2 <= 30)
                                elem->elements[elem->curr_size++] = create_tree_element(COMMAND, op_value, nullptr, nullptr);
                            else
                                text.counter = start;
                        }

                        else
                            elem->elements[elem->curr_size++] = create_tree_element(COMMAND, op_value, nullptr, nullptr);
                        break;
                    }
                    case RETURN:
                    {
                        elem->elements[elem->curr_size++] = create_tree_element(RETURN, 0, nullptr, nullptr);
                        break;
                    }
                    case BRACKET:
                    {
                        if (text.lines[text.line_counter].real_num_line % 5 != 0 && text.lines[text.line_counter].real_num_line % 5 != 3)
                            elem->elements[elem->curr_size++] = create_tree_element(BRACKET, op_value, nullptr, nullptr);
                        else
                            text.counter = start;

                        break;
                    }
                    case FIG_BRACKET:
                    {
                        elem->elements[elem->curr_size++] = create_tree_element(FIG_BRACKET, op_value, nullptr, nullptr);
                        break;
                    }
                    default:
                        break;
                }
            }

            free(temp_var_name);
        }
        else if (*(text.counter) == '+')
        {
            elem->elements[elem->curr_size++] = create_tree_element(OPER, ADD, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '-')
        {
            elem->elements[elem->curr_size++] = create_tree_element(OPER, SUB, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '*')
        {
            elem->elements[elem->curr_size++] = create_tree_element(OPER, MUL, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '/')
        {
            elem->elements[elem->curr_size++] = create_tree_element(OPER, DIV, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '^')
        {
            elem->elements[elem->curr_size++] = create_tree_element(OPER, POW, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '{' && (text.lines[text.line_counter].real_num_line % 5 == 0 || text.lines[text.line_counter].real_num_line % 5 == 3))
        {
            elem->elements[elem->curr_size++] = create_tree_element(BRACKET, OPEN, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '[' && (text.lines[text.line_counter].real_num_line % 5 == 0 || text.lines[text.line_counter].real_num_line % 5 == 3))
        {
            elem->elements[elem->curr_size++] = create_tree_element(BRACKET, CLOSE, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == ']')
        {
            elem->elements[elem->curr_size++] = create_tree_element(FIG_BRACKET, OPEN, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '(')
        {
            elem->elements[elem->curr_size++] = create_tree_element(FIG_BRACKET, CLOSE, nullptr, nullptr);
            text.counter++;
        }
        else if (strncmp(text.counter, "!!!", 2) == 0 && text.lines[text.line_counter].real_num_line % 2 == 0)
        {
            elem->elements[elem->curr_size++] = create_tree_element(BUNCH, 0, nullptr, nullptr);
            text.counter += 3;
        }
        else if (*(text.counter) == '?' && text.lines[text.line_counter].real_num_line % 2 == 1)
        {
            elem->elements[elem->curr_size++] = create_tree_element(BUNCH, 0, nullptr, nullptr);
            text.counter++;
        }
        else if (strncmp(text.counter, "==", 2) == 0)
        {
            elem->elements[elem->curr_size++] = create_tree_element(CONDITION, JE, nullptr, nullptr);
            text.counter += 2;
        }
        else if (strncmp(text.counter, "!=", 2) == 0)
        {
            elem->elements[elem->curr_size++] = create_tree_element(CONDITION, JNE, nullptr, nullptr);
            text.counter += 2;
        }
        else if (strncmp(text.counter, "<=", 2) == 0)
        {
            elem->elements[elem->curr_size++] = create_tree_element(CONDITION, JBE, nullptr, nullptr);
            text.counter += 2;
        }
        else if (strncmp(text.counter, ">=", 2) == 0)
        {
            elem->elements[elem->curr_size++] = create_tree_element(CONDITION, JAE, nullptr, nullptr);
            text.counter += 2;
        }
        else if (*(text.counter) == '<')
        {
            elem->elements[elem->curr_size++] = create_tree_element(CONDITION, JB, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '>')
        {
            elem->elements[elem->curr_size++] = create_tree_element(CONDITION, JA, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == '=' && text.lines[text.line_counter].length > 30)
        {
            elem->elements[elem->curr_size++] = create_tree_element(COMMAND, ASSIGN, nullptr, nullptr);
            text.counter++;
        }
        else if (*(text.counter) == ',')
        {
            elem->elements[elem->curr_size++] = create_tree_element(ENUM, 0, nullptr, nullptr);
            text.counter++;
        }

        SKIP_SPACES
    }

    int counter = 0;

    tree->root = create_prog_tree(elem, &counter);

    destruct_text(&text);

    int size_tree = 0;
    recalc_size_tree(tree->root, &size_tree);
    tree->tree_size = size_tree;

    ASSERT_TREE_OK_VOID
}

int is_keyword (char *temp_var_name, int *type)
{
    *type = NUM;

    if (strncmp(temp_var_name, "nol", 3) == 0)
        return 0;

    *type = BRACKET;

    if (strncmp(temp_var_name, "_SLAVA_", 7) == 0)
        return OPEN;

    if (strncmp(temp_var_name, "_RUSAM_", 7) == 0)
        return CLOSE;

    *type = FIG_BRACKET;

    if (strncmp(temp_var_name, "SHUE", 4) == 0)
        return OPEN;

    if (strncmp(temp_var_name, "PPSH", 4) == 0)
        return CLOSE;

    *type = FUNC;

    if (strncmp(temp_var_name, "shizus", 6) == 0)
        return SIN;
    if (strncmp(temp_var_name, "jesus", 5) == 0)
        return COS;
    else if (strncmp(temp_var_name, "trigger", 7) == 0)
        return TG;
    else if (strncmp(temp_var_name, "ctrigger", 8) == 0)
        return CTG;
    else if (strncmp(temp_var_name, "ork_shizus", 10) == 0)
        return ARCSIN;
    else if (strncmp(temp_var_name, "ork_jesus", 9) == 0)
        return ARCCOS;
    else if (strncmp(temp_var_name, "ork_trigger", 11) == 0)
        return ARCTG;
    else if (strncmp(temp_var_name, "ork_ctrigger", 12) == 0)
        return ARCCTG;
    else if (strncmp(temp_var_name, "shirinka", 8) == 0)
        return SH;
    else if (strncmp(temp_var_name, "chirinka", 8) == 0)
        return CH;
    else if (strncmp(temp_var_name, "thursday", 8) == 0)
        return TH;
    else if (strncmp(temp_var_name, "anime_govno", 11) == 0)
        return CTH;
    else if (strncmp(temp_var_name, "luna", 4) == 0)
        return LN;
    else if (strncmp(temp_var_name, "chekushka", 9) == 0)
        return DIFF;
    else if (strncmp(temp_var_name, "tselkoviy", 9) == 0)
        return SCAN;
    else if (strncmp(temp_var_name, "zaserushka", 10) == 0)
        return PRINT;
    else if (strncmp(temp_var_name, "perdushka", 9) == 0)
        return POWER;

    *type = MAIN;

    if (strncmp(temp_var_name, "zhuchok", 7) == 0)
        return MAIN;

    *type = COMMAND;

    if (strncmp(temp_var_name, "pornushka", 9) == 0)
        return IF;

    if (strncmp(temp_var_name, "mudachok", 8) == 0)
        return WHILE;

    if (strncmp(temp_var_name, "poluchaet", 9) == 0)
        return ASSIGN;

    *type = RETURN;

    if (strncmp(temp_var_name, "hui_na_vorotnichok", strlen("hui_na_vorotnichok")) == 0)
        return RETURN;

    return -1;

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

void find_last_var (bin_tree_elem *element, int *var)
{
    if (element->left != nullptr)
        find_last_var(element->left, var);

    if (element->type == VAR)
        *var = (int) element->value;

    if (element->right != nullptr)
        find_last_var(element->right, var);
}

bin_tree_elem *diff_subst (bin_tree_elem *element)
{
    bin_tree_elem *derivative = nullptr;
    int var_value = -1;

    find_last_var(element, &var_value);

    if (var_value == -1)
        derivative = create_tree_element(NUM, 0, nullptr, nullptr);
    else
        derivative = derivate_tree(element, var_value);

    delete_tree_elem(element);

    return derivative;
}

bin_tree_elem *create_prog_tree (elements *elem, int *counter)
{
    bin_tree_elem *bunch  = nullptr;
    bin_tree_elem *vertex = elem->elements[*counter];
    (*counter)++;

    if (vertex->type == MAIN)
        vertex = create_main_tree(elem, counter);
    else if (vertex->type == VAR && elem->elements[*counter]->type == BRACKET && (int) elem->elements[*counter]->value == OPEN)
        vertex = create_user_func_tree(elem, counter);
    else if (vertex->type == VAR && elem->elements[*counter]->type == COMMAND && (int) elem->elements[*counter]->value == ASSIGN)
    {
        vertex        = elem->elements[*counter];
        vertex->left  = elem->elements[*counter - 1];
        (*counter)++;
        vertex->right = create_e_tree(elem, counter);
        (*counter)++;
        vertex->left->type = GLOB_VAR;
    }

    bunch = create_tree_element(BUNCH, 0, nullptr, nullptr);

    bunch->left = vertex;

    if (elem->curr_size > *counter)
        bunch->right = create_prog_tree(elem, counter);

    return bunch;
}

bin_tree_elem *create_user_func_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex = elem->elements[*counter - 1];

    if (vertex->type == VAR && elem->elements[*counter]->type == BRACKET && (int) elem->elements[*counter]->value == OPEN)
    {
        (*counter)++;
        vertex->left = create_var_func_tree(elem, counter);

        bin_tree_elem *bracket = elem->elements[*counter];
        (*counter)++;

        bracket = elem->elements[*counter];
        (*counter)++;

        if (bracket->type == FIG_BRACKET && (int) bracket->value == OPEN)
        {
            vertex->right = create_body_tree(elem, counter);

            bracket = elem->elements[*counter];
            (*counter)++;
        }
    }

    vertex->type = USER_FUNC;

    return vertex;
}

bin_tree_elem *create_var_func_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex = elem->elements[*counter];
    (*counter)++;

    if (vertex->type == VAR && elem->elements[*counter]->type == ENUM)
    {
        (*counter)++;
        vertex->left = create_var_func_tree(elem, counter);
    }

    return vertex;
}

bin_tree_elem *create_param_func_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex = create_e_tree(elem, counter);

    bin_tree_elem *bunch = create_tree_element(BUNCH, 0, nullptr, nullptr);

    bunch->right = vertex;

    if (elem->elements[*counter]->type == ENUM)
    {
        (*counter)++;
        bunch->left = create_param_func_tree(elem, counter);
    }

    return bunch;
}

bin_tree_elem *create_main_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex = elem->elements[*counter - 1];

    if (vertex->type == MAIN)
    {
        bin_tree_elem *bracket = elem->elements[*counter];
        (*counter)++;

        if (bracket->type == FIG_BRACKET && (int) bracket->value == OPEN)
        {
            vertex->right = create_body_tree(elem, counter);

            bracket = elem->elements[*counter];
            (*counter)++;
        }
    }

    return vertex;
}

bin_tree_elem *create_body_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex = create_cmd_tree(elem, counter);
    bin_tree_elem *bunch  = nullptr;

    if ((vertex->type == FUNC && ((int) vertex->value == SCAN || (int) vertex->value == PRINT)) || vertex->type == RETURN || vertex->type == USER_FUNC || (vertex->type == COMMAND && ((int) vertex->value != IF && (int) vertex->value != WHILE)))
    {
        bunch = elem->elements[*counter];
        (*counter)++;
    }
    else
        bunch = create_tree_element(BUNCH, 0, nullptr, nullptr);

    bunch->left = vertex;

    if (elem->elements[*counter]->type != FIG_BRACKET)
        bunch->right = create_body_tree(elem, counter);

    return bunch;
}

bin_tree_elem *create_cmd_tree (elements *elem, int *counter)
{
    bin_tree_elem *command = nullptr;

    if (elem->elements[*counter]->type == COMMAND && ((int) elem->elements[*counter]->value == IF || (int) elem->elements[*counter]->value == WHILE))
    {
        command = elem->elements[*counter];
        (*counter)++;

        bin_tree_elem *bracket1 = elem->elements[*counter];
        (*counter)++;

        bin_tree_elem *condition_left = create_e_tree(elem, counter);

        undertree_optimize(condition_left);

        condition_left = diff_subst(condition_left);

        bin_tree_elem *condition = elem->elements[*counter];
        (*counter)++;

        bin_tree_elem *condition_right = create_e_tree(elem, counter);

        undertree_optimize(condition_right);

        condition_right = diff_subst(condition_right);

        bin_tree_elem *bracket2 = elem->elements[*counter];
        (*counter)++;

        condition->left  = condition_left;
        condition->right = condition_right;
        command->left = condition;

        bracket1 = elem->elements[*counter];
        (*counter)++;

        command->right = create_body_tree(elem, counter);

        bracket2 = elem->elements[*counter];
        (*counter)++;

    }
    else if (elem->elements[*counter]->type == VAR && elem->elements[*counter + 1]->type == BRACKET && (int) elem->elements[*counter + 1]->value == OPEN)
    {
        command = elem->elements[*counter];
        command->type = USER_FUNC;

        *counter += 2;

        command->left = create_param_func_tree(elem, counter);

        (*counter)++;
    }
    else if (elem->elements[*counter]->type == RETURN)
    {
        command = elem->elements[*counter];
        (*counter)++;

        if (elem->elements[*counter]->type != BUNCH)
        {
            command->left = create_e_tree(elem, counter);

            undertree_optimize(command->left);

            command->left = diff_subst(command->left);
        }
    }
    else if (elem->elements[*counter]->type == FUNC && (int) elem->elements[*counter]->value == SCAN)
    {
        command = elem->elements[*counter];
        (*counter)++;

        bin_tree_elem *bracket = elem->elements[*counter];
        (*counter)++;

        command->left = elem->elements[*counter];
        (*counter)++;

        bracket = elem->elements[*counter];
        (*counter)++;
    }
    else if (elem->elements[*counter]->type == FUNC && (int) elem->elements[*counter]->value == PRINT)
    {
        command = elem->elements[*counter];
        (*counter)++;

        bin_tree_elem *bracket = elem->elements[*counter];
        (*counter)++;

        command->left = create_e_tree(elem, counter);

        undertree_optimize(command->left);

        command->left = diff_subst(command->left);

        bracket = elem->elements[*counter];
        (*counter)++;
    }
    else
    {
        bin_tree_elem *expression1 = create_e_tree(elem, counter);

        command = elem->elements[*counter];
        (*counter)++;

        bin_tree_elem *expression2 = create_e_tree(elem, counter);

        command->left  = expression1;
        command->right = expression2;

        undertree_optimize(command->right);

        command->right = diff_subst(command->right);
    }

    return command;
}

bin_tree_elem *create_n_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex = nullptr;

    if (elem->elements[*counter]->type == NUM)
    {
        vertex = elem->elements[*counter];
        (*counter)++;
    }
    else if (elem->elements[*counter]->type == VAR && elem->elements[*counter + 1]->type == BRACKET && (int) elem->elements[*counter + 1]->value == OPEN)
    {
        vertex = elem->elements[*counter];
        vertex->type = USER_FUNC;

        *counter += 2;

        vertex->left = create_param_func_tree(elem, counter);

        bin_tree_elem *bracket = elem->elements[*counter];
        (*counter)++;

    }
    else if (elem->elements[*counter]->type == VAR)
    {
        vertex = elem->elements[*counter];
        (*counter)++;
    }
    else if (elem->elements[*counter]->type == FUNC && (int) elem->elements[*counter]->value != DIFF && (int) elem->elements[*counter]->value != POWER)
    {
        bin_tree_elem *func_elem = elem->elements[*counter];
        (*counter)++;

        if (*counter < elem->curr_size && elem->elements[*counter]->type == BRACKET && (int) elem->elements[*counter]->value == OPEN)
        {
            (*counter)++;

            vertex = create_e_tree(elem, counter);

            if (*counter < elem->curr_size && elem->elements[*counter]->type == BRACKET && (int) elem->elements[*counter]->value == CLOSE)
                (*counter)++;
        }

        func_elem->left = vertex;
        vertex = func_elem;
    }
    else if (elem->elements[*counter]->type == FUNC && (int) elem->elements[*counter]->value == DIFF)
    {
        bin_tree_elem *func_elem = elem->elements[*counter];
        bin_tree_elem *formula = nullptr;
        (*counter)++;

        if (*counter < elem->curr_size && elem->elements[*counter]->type == BRACKET && (int) elem->elements[*counter]->value == OPEN)
        {
            (*counter)++;

            vertex = elem->elements[*counter];

            *counter += 2;

            formula = create_e_tree(elem, counter);

            if (*counter < elem->curr_size && elem->elements[*counter]->type == BRACKET && (int) elem->elements[*counter]->value == CLOSE)
                (*counter)++;
        }

        func_elem->left  = vertex;
        func_elem->right = formula;
        vertex = func_elem;
    }
    else if (elem->elements[*counter]->type == FUNC && (int) elem->elements[*counter]->value == POWER)
    {
        bin_tree_elem *func_elem = elem->elements[*counter];
        bin_tree_elem *power = nullptr;
        (*counter)++;

        if (*counter < elem->curr_size && elem->elements[*counter]->type == BRACKET && (int) elem->elements[*counter]->value == OPEN)
        {
            (*counter)++;

            vertex = create_e_tree(elem, counter);

            (*counter)++;

            power = create_e_tree(elem, counter);

            if (*counter < elem->curr_size && elem->elements[*counter]->type == BRACKET && (int) elem->elements[*counter]->value == CLOSE)
                (*counter)++;
        }

        func_elem->left  = vertex;
        func_elem->right = power;
        vertex = func_elem;
    }

    return vertex;
}

bin_tree_elem *read_w_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex = create_p_tree(elem, counter);

    if (*counter < elem->curr_size && elem->elements[*counter]->type == OPER && (int) elem->elements[*counter]->value == POW)
    {
        bin_tree_elem *new_vertex = vertex;

        vertex = elem->elements[*counter];

        (*counter)++;

        vertex->left  = new_vertex;
        vertex->right = read_w_tree(elem, counter);
    }

    return vertex;
}

bin_tree_elem *create_w_tree (elements *elem, int *counter)
{
    bin_tree_elem *prev_vertex = nullptr;
    bin_tree_elem *vertex      = create_p_tree(elem, counter);

    if (*counter < elem->curr_size && elem->elements[*counter]->type == OPER && (int) elem->elements[*counter]->value == POW)
    {
        prev_vertex = vertex;

        vertex = elem->elements[*counter];

        vertex->left = prev_vertex;

        (*counter)++;

        vertex->right = read_w_tree(elem, counter);
    }

    return vertex;
}

bin_tree_elem *create_t_tree (elements *elem, int *counter)
{
    bin_tree_elem *prev_vertex = nullptr;
    bin_tree_elem *vertex      = create_w_tree(elem, counter);

    if (*counter < elem->curr_size && elem->elements[*counter]->type == OPER && ((int) elem->elements[*counter]->value == MUL || (int) elem->elements[*counter]->value == DIV))
    {
        while (*counter < elem->curr_size && elem->elements[*counter]->type == OPER && ((int) elem->elements[*counter]->value == MUL || (int) elem->elements[*counter]->value == DIV))
        {
            prev_vertex = vertex;

            vertex = elem->elements[*counter];

            vertex->left = prev_vertex;

            (*counter)++;

            vertex->right = create_w_tree(elem, counter);
        }
    }

    return vertex;
}

bin_tree_elem *create_p_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex = nullptr;

    if (*counter < elem->curr_size && elem->elements[*counter]->type == BRACKET && (int) elem->elements[*counter]->value == OPEN)
    {
        (*counter)++;

        vertex = create_e_tree(elem, counter);

        if (*counter < elem->curr_size && elem->elements[*counter]->type == BRACKET && (int) elem->elements[*counter]->value == CLOSE)
            (*counter)++;
    }
    else
        vertex = create_n_tree(elem, counter);

    return vertex;
}

bin_tree_elem *create_e_tree (elements *elem, int *counter)
{
    bin_tree_elem *vertex      = nullptr;
    bin_tree_elem *prev_vertex = nullptr;

    int sign = 0;

    if (elem->elements[*counter]->type == OPER && (int) elem->elements[*counter]->value == SUB)
    {
        sign = -1;
        (*counter)++;
    }

    vertex = create_t_tree(elem, counter);

    if (sign == -1)
        vertex = MULTIPLY(CR_NUM(-1), vertex);

    if (*counter < elem->curr_size && elem->elements[*counter]->type == OPER && ((int) elem->elements[*counter]->value == SUB || (int) elem->elements[*counter]->value == ADD))
    {
        while (*counter < elem->curr_size && elem->elements[*counter]->type == OPER && ((int) elem->elements[*counter]->value == SUB || (int) elem->elements[*counter]->value == ADD))
        {
            prev_vertex = vertex;

            vertex = elem->elements[*counter];

            vertex->left = prev_vertex;

            (*counter)++;

            vertex->right = create_t_tree(elem, counter);
        }
    }

    return vertex;
}

void syntax_error(text_t *text, elements *elem, int line, const char *file)
{
    assert(elem);
    printf("Syntax error [%d line in cpp] (%s).\n"
           "Process of file-reading terminated. Please, use the right syntax.\n", line, file);
    printf("You have an error in line %d in file: ", text->lines[text->line_counter].real_num_line + 1);

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

    for (int i = 0; i < elem->curr_size; i++)
        free(elem->elements[i]);
}
