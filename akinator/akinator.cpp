#include "akinator.h"

#define READ_PHRASE(buffer)                     \
    fflush(stdin);                              \
    fgets(buffer, MAX_CMD_LENGTH, stdin);       \
    buffer[strlen(buffer) - 1] = 0;
    

void akinator_menu (const char *base)
{
    FILE *source = fopen(base, "rb");

    if (!source)
    {
        printf("\n!!There is no file \"akinator_base.txt\"...\n"
                "Program was terminated...\n");

        abort();
    }

    bin_tree tree = {};
    construct_tree(&tree, "tree");

    fill_tree_from_base(&tree, source);

    fclose(source);

    printf("Привет, меня зовут Акинатор!\n"
           "Введите, пожалуйста, режим, с которым вы хотите работать:))\n"
           "1 - Давай поиграем\n"
           "2 - Выведу все отличия и сходства между указанными предметами (чтобы посмотреть список объектов, которые я знаю, просмотрите их в режиме 4)\n"
           "3 - Опишу все свойства предмета (чтобы посмотреть список объектов, которые я знаю, просмотрите их в режиме 4)\n"
           "4 - Показать всё, что я знаю\n\n");

    txSpeak("Привет меня зовут Акинатор! Введите пожалуйста режим с которым вы хотите работать!");

    printf("Выбранный режим: ");

    char *regime = (char *) calloc(MAX_CMD_LENGTH, sizeof(char));

    READ_PHRASE(regime)

    while(strlen(regime) > 1 || regime[0] < '1' || regime[0] > '4')
    {
        printf("\nОшибка! Пожалуйста, введите число от 1 до 4, выбрав режим, с которым вы хотите со мной работать!\n");
        txSpeak("Ошибка! Введите число от одного до четырёх");
        READ_PHRASE(regime)
    }

    int continue_status = OFF;

    while (1)
    {
        if (strcmp(regime, "1") == 0)
        {
            object_guess(&tree);

            source = fopen(base, "wb"); // now for writing, not reading
            fill_akinator_base(&tree, source);
            fclose(source);
        }
        else if (strcmp(regime, "2") == 0)
            object_compare(&tree);
        else if (strcmp(regime, "3") == 0)
            object_definition(&tree);
        else if (strcmp(regime, "4") == 0)
            show_base(&tree);
        else
            txSpeak("\vСорри но я слишком глуп чтобы понять, что вы ввели");

        txSpeak("\v\nЕсли хотите продолжить пользоваться мной, введите \"1\", иначе \"0\".\n");

        READ_PHRASE(regime)

        while (strlen(regime) > 1 || (regime[0] != '0' && regime[0] != '1'))
        {
            txSpeak("\v\nЯ вас не поняла, введите, пожалуйста \"1\" или \"0\".\n");
            READ_PHRASE(regime)
        }

        if (strlen(regime) == 1 && regime[0] == '1')
        {
            txSpeak("\v\nВыберите режим: ");

            READ_PHRASE(regime)

            while(strlen(regime) > 1 || isdigit(regime[0]) == 0)
            {
                txSpeak("\v\nОшибка! Пожалуйста, введите число от 1 до 4, выбрав режим, с которым вы хотите со мной работать!\n");
                READ_PHRASE(regime)
            }
        }
        else
            break;
    }

    free(regime);

    source = fopen(base, "wb"); // now for writing, not reading
    fill_akinator_base(&tree, source);
    fclose(source);

    destruct_tree(&tree);
}

void fill_tree_from_base (bin_tree *tree, FILE *base)
{
    assert(base);

    int file_symbols = length_of_file(base);

    char *buffer = (char *) calloc(file_symbols + 1, sizeof(char));
    fread(buffer, sizeof(char), file_symbols, base);

    int counter = 0;

    while (buffer[counter++] != '[');
    counter--;

    tree->root = create_tree(tree, buffer, &counter);

    if (tree->root == nullptr)
    {
        printf("\n!!There is an error in akinator base!!\n"
                "Program was terminated...\n");

        abort();
    }

    counter = 0;

    add_prev_links(tree->root, &counter);

    tree->tree_size = counter;

    free(buffer);
    fclose(base);
}

int length_of_file (FILE *file)
{
    assert(file);

    fseek(file, 0, SEEK_END);
    int length_file = ftell(file);
    fseek(file, 0, SEEK_SET);

    return length_file;
}

bin_tree_elem *create_tree (bin_tree *tree, char *buffer, int *counter)
{
    while(isspace(buffer[*counter]))
        (*counter)++;

    if (buffer[*counter] == '[')
        (*counter)++;

    while(isspace(buffer[*counter]))
        (*counter)++;

    bin_tree_elem *element = (bin_tree_elem *) calloc(1, sizeof(bin_tree_elem));

    if (buffer[*counter] == '`' || buffer[*counter] == '?')
    {
        (*counter)++;

        int temp_counter = *counter;

        while(buffer[temp_counter] != '`' && buffer[temp_counter] != '?')
            temp_counter++;

        element->data  = (char *) calloc(temp_counter - *counter + 1, sizeof(char));
        strncpy(element->data, buffer + *counter, temp_counter - *counter);

        element->left  = nullptr;
        element->right = nullptr;
        element->prev  = nullptr;

        *counter = temp_counter + 1;

        if (buffer[*counter - 1] == '?')
        {
            element->left  = create_tree(tree, buffer, counter);
            element->right = create_tree(tree, buffer, counter);

            if (element->left == nullptr || element->right == nullptr)
            {
                printf("\n!!There is an error in akinator base!!\n"
                       "Program was terminated...\n");

                abort();
            }
        }
    }

    while(isspace(buffer[*counter]))
        (*counter)++;

    if (buffer[*counter] == ']')
    {
        (*counter)++;
        return element;
    }

    return nullptr;
}

void add_prev_links (bin_tree_elem *element, int *counter)
{
    (*counter)++;

    if (element->left != nullptr)
    {
        element->left->prev = element;

        add_prev_links(element->left, counter);
    }

    if (element->right != nullptr)
    {
        element->right->prev = element;

        add_prev_links(element->right, counter);
    }
}

void object_guess (bin_tree *tree)
{
    ASSERT_TREE_OK_VOID

    printf("Ваш выбор помог нам в развитии проекта!!!\n");
    txSpeak("Прекрасно");
    txSpeak("\vЗагадайте любой предмет и честно ответьте на все нижеперечисленные вопросы");

    ask_questions(tree, tree->root);
}

void ask_questions (bin_tree *tree, bin_tree_elem *element)
{
    txSpeak("\v\nПожалуйста, ответьте \"Да\" или \"Нет\"\n"
            "Это %s?\n", element->data);

    char *answer = (char *) calloc(MAX_CMD_LENGTH, sizeof(char));

    READ_PHRASE(answer)

    while(strcmp(answer, "Да") != 0 && strcmp(answer, "Нет") != 0)
    {
        txSpeak("\v\nОшибка! Пожалуйста, ответьте \"Да\" или \"Нет\"\n");

        READ_PHRASE(answer)
    }

    if (strcmp(answer, "Да") == 0)
    {
        free(answer);

        if (element->right == nullptr)
        {
            txSpeak("\vДа я просто пушка!\n");
            return;
        }
        else
            ask_questions(tree, element->right);
    }
    else if (strcmp(answer, "Нет") == 0)
    {
        free(answer);

        if (element->left == nullptr)
        {
            printf("\nО нет! Я не знаю что это...\nПожалуйста, введите ваше загаданное слово: ");
            txSpeak("Я не знаю вашего слова! Пожалуйста введите его:");
            
            char *new_word = (char *) calloc(MAX_CMD_LENGTH, sizeof(char));
            
            READ_PHRASE(new_word)

            bin_tree_elem *new_elem1 = (bin_tree_elem *) calloc(1, sizeof(bin_tree_elem));

            new_elem1->left  = nullptr;
            new_elem1->right = nullptr;
            new_elem1->prev  = element;
            new_elem1->data = (char *) calloc(strlen(element->data) + 1, sizeof(char));
            strcpy(new_elem1->data, element->data);

            bin_tree_elem *new_elem2 = (bin_tree_elem *) calloc(1, sizeof(bin_tree_elem));

            new_elem2->left  = nullptr;
            new_elem2->right = nullptr;
            new_elem2->prev  = element;
            new_elem2->data = (char *) calloc(strlen(new_word) + 1, sizeof(char));
            strcpy(new_elem2->data, new_word);

            element->left  = new_elem1;
            element->right = new_elem2;

            printf("\nПримеры:\n"
                   "Это...\n"
                   "состоит из металла\n"
                   "связано с Украиной\n"
                   "красного цвета\n\n");

            txSpeak("\vПожалуйста, введите признак, который подходит \"%s\" и не подходит \"%s\" (согласно примерам выше): ", new_elem2->data, element->data);

            READ_PHRASE(new_word)

            size_t word_len = strlen(new_word);
            realloc(element->data, MAX_CMD_LENGTH);
            new_word[word_len] = 0;

            strncpy(element->data, new_word, strlen(new_word));

            tree->tree_size += 2;

            free(new_word);
        }
        else
            ask_questions(tree, element->left);
    }
}

void object_compare (bin_tree *tree)
{
    ASSERT_TREE_OK_VOID

    txSpeak("\vПривет! Я готова показать, что я не дальтоник.\n"
            "Введите два слова, которые вы хотите сравнить!\n");

    char *word1 = (char *) calloc(MAX_CMD_LENGTH, sizeof(char));
    char *word2 = (char *) calloc(MAX_CMD_LENGTH, sizeof(char));

    READ_PHRASE(word1)
    READ_PHRASE(word2)

    while (strcmp(word1, word2) == 0)
    {
        txSpeak("\vОшибка! Пожалуйста, введите именно два РАЗНЫХ объекта.\n");

        READ_PHRASE(word1)
        READ_PHRASE(word2)
    }

    bin_tree_elem *element1 = search_word(tree->root, word1);
    bin_tree_elem *element2 = search_word(tree->root, word2);

    while (element1 == nullptr || element2 == nullptr)
    {
        if (element1 == nullptr)
        {
            printf("Кажется, я впервые вижу первый предмет... Я не знаю ничего про него, отстаньте (введите два предмета заново)!\n");
            txSpeak("Я хз, что означает первый предмет");
        }

        if (element2 == nullptr)
        {
            printf("Кажется, я впервые вижу второй предмет... Я не знаю ничего про него, отстаньте (введите два предмета заново)!\n");
            txSpeak("Я хз что означает второй предмет");
        }

        READ_PHRASE(word1)
        READ_PHRASE(word2)

        while (strcmp(word1, word2) == 0)
        {
            txSpeak("\vОшибка! Пожалуйста, введите именно два РАЗНЫХ объекта.\n");

            READ_PHRASE(word1)
            READ_PHRASE(word2)
        }

        element1 = search_word(tree->root, word1);
        element2 = search_word(tree->root, word2);
    }

    int length1 = 0;
    int length2 = 0;
    int counter = 0;

    for (bin_tree_elem *element = element1; element != nullptr; element = element->prev)
        length1++;

    for (bin_tree_elem *element = element2; element != nullptr; element = element->prev)
        length2++;

    bin_tree_elem **path1 = (bin_tree_elem **) calloc(length1, sizeof(bin_tree_elem *));
    bin_tree_elem **path2 = (bin_tree_elem **) calloc(length2, sizeof(bin_tree_elem *));

    counter = length1 - 1;

    for (bin_tree_elem *element = element1; element != nullptr; element = element->prev)
    {
        path1[counter] = element;
        counter--;
    }

    counter = length2 - 1;

    for (bin_tree_elem *element = element2; element != nullptr; element = element->prev)
    {
        path2[counter] = element;
        counter--;
    }

    if (path1[1] != path2[1] && length1 != 2 && length2 != 2)
        txSpeak("\vПолучилось, что %s и %s ничем не похожи (увы)\n", word1, word2);
    else if (path1[1] != path2[1] && (length1 == 2 || length2 == 2))
    {
        if (path1[1]->prev->left == path1[1])
            txSpeak("\vОбъекты ничем не похожи, но известно, что %s %s, но %s не %s\n", element2->data, path2[0]->data, element1->data, path2[0]->data);
        else
            txSpeak("\vОбъекты ничем не похожи, но известно, что %s %s, но %s не %s\n", element1->data, path2[0]->data, element2->data, path2[0]->data);
    }
    else
    {
        counter = 0;

        printf("\nОбъекты похожи тем, что каждый из них (%s и %s):", element1->data, element2->data);
        txSpeak("Объекты похожи тем, что каждый из них");

        while (path1[counter] == path2[counter])
        {
            if (path1[counter + 1] == path2[counter + 1] && counter + 1 < length1 && counter + 1 < length2)
            {
                if (path1[counter]->left == path1[counter + 1])
                    txSpeak("\v не %s,", path1[counter++]->data);
                else
                    txSpeak("\v %s,", path1[counter++]->data);
            }
            else
                break;
        }

        if (path1[counter]->left == element1)
            txSpeak("\v но %s %s, а %s не %s.\n", element1->data, path1[counter]->data, element2->data, path1[counter]->data);
        else
            txSpeak("\v но %s %s, а %s не %s.\n", element2->data, path1[counter]->data, element1->data, path1[counter]->data);
    }

    free(path1);
    free(path2);
    free(word1);
    free(word2);
}

void object_definition (bin_tree *tree)
{
    ASSERT_TREE_OK_VOID

    txSpeak("\vКу! Напиши мне предмет, который ты хочешь описать, и я сделаю это за тебя!\n");

    char *word = (char *) calloc(MAX_CMD_LENGTH, sizeof(char));

    READ_PHRASE(word)

    bin_tree_elem *elem_word = search_word(tree->root, word);

    if (elem_word == nullptr)
        txSpeak("\vКажется я впервые вижу этот предмет... Я не знаю ничего про него, отстаньте!\n");
    else
    {
        txSpeak("\vЧётко. Вот, что я знаю о %s:\n", word);
        print_element_properties(elem_word);
    }
}

bin_tree_elem *search_word (bin_tree_elem *element, char *word)
{
    bin_tree_elem *temp1 = nullptr;
    bin_tree_elem *temp2 = nullptr;

    if (element->left != nullptr)
        temp1 = search_word(element->left, word);
    if (element->right != nullptr)
        temp2 = search_word(element->right, word);

    if (element->left == nullptr && element->right == nullptr)
    {
        if (strcmp(word, element->data) == 0)
            return element;
        else
            return nullptr;
    }

    if (temp1 != nullptr)
        return temp1;

    if (temp2 != nullptr)
        return temp2;

    return nullptr;
}

void print_element_properties (bin_tree_elem *element)
{
    if (element->prev == nullptr)
        return;

    if (element->prev->left == element)
        txSpeak("\v- не %s\n", element->prev->data);
    else if (element->prev->right == element)
        txSpeak("\v- %s\n", element->prev->data);

    print_element_properties(element->prev);
}

void show_base (bin_tree *tree)
{
    ASSERT_TREE_OK_VOID

    FILE *graphviz = fopen("graph_base.dot", "wb");
    assert(graphviz);

    fprintf(graphviz, "digraph binary_tree {\n");
    fprintf(graphviz, "  node [shape = \"circle\", style = \"filled\", fillcolor = \"blue\", fontcolor = \"#FFFFFF\", margin = \"0.01\"];\n");
    fprintf(graphviz, "  rankdir = \"TB\";\n\n");
    fprintf(graphviz, "label = \"База Акинатора\";\n");

    print_base_tree(tree->root, graphviz);

    fprintf(graphviz, "}");

    fclose(graphviz);

#ifdef UTF8_CONV
    system(PATH_CODE);
    system(GRAPH_BASE);
    system(DELETE_OLD);
    system(RENAME_NEW);
#else
    system(GRAPH_BASE_ALT);
#endif

    system(PRINT_BASE);
}

void print_base_tree (bin_tree_elem *element, FILE *output)
{
    if (element->left == nullptr && element->right == nullptr)
        fprintf(output, "  \"%s\" [shape = \"ellipse\", fillcolor = \"red\", label = \"%s\"];\n", element->data, element->data);
    else
        fprintf(output, "  \"%s\" [shape = \"ellipse\", label = \"%s\"];\n", element->data, element->data);

    if (element->left != nullptr)
    {
        if (element->left->prev == element)
        {
            fprintf(output, "  \"%s\"->\"%s\";\n", element->data, element->left->data);
        }
        else
        {
            fprintf(output, "  edge [color=\"#FE6200\"];\n");
            fprintf(output, "  \"%s\"->\"%s\";\n", element->data, element->left->data);
            fprintf(output, "  edge [color=\"#000000\"];\n");
        }

        print_base_tree(element->left, output);
    }

    if (element->right != nullptr)
    {
        if (element->right->prev == element)
        {
            fprintf(output, "  \"%s\"->\"%s\";\n", element->data, element->right->data);
        }
        else
        {
            fprintf(output, "  edge [color=\"#FE6200\"];\n");
            fprintf(output, "  \"%s\"->\"%s\";\n", element->data, element->right->data);
            fprintf(output, "  edge [color=\"#000000\"];\n");
        }

        print_base_tree(element->right, output);
    }
}

void fill_akinator_base (bin_tree *tree, FILE *base)
{
    ASSERT_TREE_OK_VOID
    assert(base);

    fprintf(base, "{description}\n"
                  "{version 1.0}\n"
                  "{RUS}\n\n");

    fill_base_elements(tree->root, base, 0);
}

void fill_base_elements (bin_tree_elem *element, FILE *base, int indent)
{
    print_indent(base, indent);

    if (element->left == nullptr && element->right == nullptr)
    {
        fprintf(base, "[\n");
        print_indent(base, indent + 1);
        fprintf(base, "`%s`\n", element->data);
        print_indent(base, indent);
        fprintf(base, "]\n");

        return;
    }

    fprintf(base, "[\n");
    print_indent(base, indent + 1);
    fprintf(base, "?%s?\n", element->data);

    if (element->left != nullptr)
        fill_base_elements(element->left, base, indent + 1);

    if (element->right != nullptr)
        fill_base_elements(element->right, base, indent + 1);

    print_indent(base, indent);

    fprintf(base, "]\n");
}

inline void print_indent (FILE *base, int indent)
{
    assert(base);
    
    for (int i = 0; i < indent; i++)
        fprintf(base, "\t");
}
