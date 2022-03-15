#include "akinator.h"

void construct_tree (bin_tree *tree, const char *name)
{
    assert(tree);
    assert(name);

    tree->root = (bin_tree_elem *) calloc(1, sizeof(bin_tree_elem));
    tree->tree_size = 0;

    tree->root->left  = nullptr;
    tree->root->right = nullptr;
    tree->root->prev  = nullptr;

    tree->error_state = 0;

    tree->tree_name = (char *) calloc(strlen(name) + 1, sizeof(char));
    strcpy(tree->tree_name, name);
}

void delete_tree_elem (bin_tree_elem *element)
{
    if (element->left != nullptr)
        delete_tree_elem(element->left);

    if (element->right != nullptr)
        delete_tree_elem(element->right);

    free(element);
}

void destruct_tree (bin_tree *tree)
{
    ASSERT_TREE_OK_VOID

    delete_tree_elem(tree->root);

    free(tree->tree_name);

    tree->tree_name = nullptr;
    tree->root      = nullptr;

    tree->error_state = 0;

    tree->tree_size = -1;
}

bin_tree_elem *insert_left_tree (bin_tree *tree, bin_tree_elem *element, char *data)
{
    ASSERT_TREE_OK
    assert(data);

    if (tree->tree_size == 0)
    {
        tree->root->data  = (char *) calloc(strlen(data) + 1, sizeof(char));
        strcpy(tree->root->data, data);

        tree->tree_size++;

        return tree->root;
    }

    if (element == nullptr)
    {
        tree->error_state = TREE_INVALID_INSERT_ELEMENT;
        dump_tree(tree);

        return nullptr;
    }

    if (element->left)
    {
        tree->error_state = TREE_REPEAT_INSERT_LEFT;
        dump_tree(tree);

        return nullptr;
    }

    bin_tree_elem *new_element = (bin_tree_elem *) calloc(1, sizeof(bin_tree_elem));

    new_element->left  = nullptr;
    new_element->right = nullptr;

    new_element->data  = (char *) calloc(strlen(data) + 1, sizeof(char));
    strcpy(new_element->data, data);

    element->left = new_element;
    new_element->prev = element;

    tree->tree_size++;

    ASSERT_TREE_OK

    return new_element;
}

bin_tree_elem *insert_right_tree (bin_tree *tree, bin_tree_elem *element, char *data)
{
    ASSERT_TREE_OK
    assert(data);

    if (tree->tree_size == 0)
    {
        tree->root->data  = (char *) calloc(strlen(data) + 1, sizeof(char));
        strcpy(tree->root->data, data);

        tree->tree_size++;

        return tree->root;
    }

    if (element == nullptr)
    {
        tree->error_state = TREE_INVALID_INSERT_ELEMENT;
        dump_tree(tree);

        return nullptr;
    }

    if (element->right)
    {
        tree->error_state = TREE_REPEAT_INSERT_RIGHT;
        dump_tree(tree);

        return nullptr;
    }

    bin_tree_elem *new_element = (bin_tree_elem *) calloc(1, sizeof(bin_tree_elem));

    new_element->left  = nullptr;
    new_element->right = nullptr;

    new_element->data  = (char *) calloc(strlen(data) + 1, sizeof(char));
    strcpy(new_element->data, data);

    element->right = new_element;
    new_element->prev = element;

    tree->tree_size++;

    ASSERT_TREE_OK

    return new_element;
}

void check_size_tree (bin_tree *tree, bin_tree_elem *element, int *counter)
{
    if (element == nullptr)
        return;

    if (*counter > tree->tree_size)
        return;

    (*counter)++;

    if (element->left != nullptr)
        check_size_tree(tree, element->left, counter);

    if (element->right != nullptr)
        check_size_tree(tree, element->right, counter);
}

int error_tree (bin_tree *tree)
{
    if (tree == nullptr)
    {
        tree->error_state = TREE_NULL_PTR;
        return TREE_NULL_PTR;
    }

    if (tree->root == nullptr)
    {
        tree->error_state = TREE_ROOT_NULL_PTR;
        return TREE_ROOT_NULL_PTR;
    }

    int counter = 0;

    check_size_tree(tree, tree->root, &counter);

    if (tree->tree_size != 0 && counter != tree->tree_size)
    {
        tree->error_state = TREE_INVALID_SIZE;
        return TREE_INVALID_SIZE;
    }

    counter = 0;

    int link_state = 0;

    check_links_tree(tree, tree->root, &counter, &link_state);

    if (link_state != 0)
    {
        tree->error_state = TREE_LINK_ERROR;
        return TREE_LINK_ERROR;
    }

    return 0;
}

void check_links_tree (bin_tree *tree, bin_tree_elem *element, int *counter, int *link_state)
{
    if (element == nullptr)
        return;

    (*counter)++;

    if (*counter > tree->tree_size)
        return;

    if (element->left != nullptr && element->left->prev != element)
        *link_state = 1;

    if (element->right != nullptr && element->right->prev != element)
        *link_state = 1;

    if (element->left != nullptr)
        check_links_tree(tree, element->left, counter, link_state);

    if (element->right != nullptr)
        check_links_tree(tree, element->right, counter, link_state);
}

void dump_tree (bin_tree *tree)
{
    FILE *output = fopen("tree_log.txt", "wb");
    assert(output);

    int error = tree->error_state;

    char *curr_time = (char *) calloc(CURRENT_TIME_LENGTH, sizeof(char));
    current_time(curr_time);

    fprintf(output, "Akinator Tree Dump in %s\n", curr_time);

    free(curr_time);

    if (error == 0)
        fprintf(output, "Binary tree (OK) [%p] \"%s\"\n", tree, tree->tree_name);
    else if (error == TREE_NULL_PTR || error == TREE_ROOT_NULL_PTR)
    {
        fprintf(output, "Binary tree (ERROR #%d: %s) [%p] \"%s\"\n", error, print_tree_error(error), tree, tree->tree_name);

        fclose(output);

        return;
    }
    else
        fprintf(output, "Binary tree (ERROR #%d: %s) [%p] \"%s\"\n", error, print_tree_error(error), tree, tree->tree_name);

    fprintf(output, "tree-size = %d\n", tree->tree_size);

    tree_print(tree->root, output);

    FILE *graphviz = fopen("graph.dot", "wb");
    assert(graphviz);

    fprintf(graphviz, "digraph binary_tree {\n");
    fprintf(graphviz, "  node [shape = \"circle\", style = \"filled\", fillcolor = \"blue\", fontcolor = \"#FFFFFF\", margin = \"0.01\"];\n");
    fprintf(graphviz, "  rankdir = \"TB\";\n\n");
    fprintf(graphviz, "  label = \"Tree Dump\";\n");

    print_graph_vertex_tree(tree->root, graphviz);

    fprintf(output, "\n");
    fclose(output);

    fprintf(graphviz, "}");
    fclose(graphviz);

    system("iconv.exe -t UTF-8 -f  CP1251 < graph.dot > graph_temp.dot");

    system("dot -Tpdf -o graph.pdf graph_temp.dot");
    system("del graph.dot");
    system("ren graph_temp.dot graph.dot");

    system("start graph.pdf");


    system("start tree_log.txt");
}

void tree_print (bin_tree_elem *element, FILE *output)
{
    if (element == nullptr)
        return;

    fprintf(output, "[%p] data = \"%.20s\", left = [%p], right = [%p], prev = [%p]\n", element, element->data, element->left, element->right, element->prev);

    if (element->left != nullptr)
        tree_print(element->left, output);

    if (element->right != nullptr)
        tree_print(element->right, output);
}

void print_graph_vertex_tree (bin_tree_elem *element, FILE *output)
{
    if (element->left == nullptr && element->right == nullptr)
        fprintf(output, "  \"%s\" [shape = \"record\", fillcolor = \"red\", label = \"{%s | prev\\n%p | <f0> pos\\n%p| left\\n%p | right\\n%p\\n}\"];\n", element->data, element->data, element->prev, element, element->left, element->right);
    else
        fprintf(output, "  \"%s\" [shape = \"record\", label = \"{%s | prev\\n%p | <f0> pos\\n%p| left\\n%p | right\\n%p\\n}\"];\n", element->data, element->data, element->prev, element, element->left, element->right);

    if (element->left != nullptr)
    {
        if (element->left->prev == element)
        {
            fprintf(output, "  \"%s\"->\"%s\";\n", element->data, element->left->data);
            fprintf(output, "  \"%s\"->\"%s\";\n", element->left->data, element->data);
        }
        else
        {
            fprintf(output, "  edge [color=\"#FE6200\"];\n");
            fprintf(output, "  \"%s\"->\"%s\";\n", element->data, element->left->data);
            fprintf(output, "  \"%s\"->\"%s\";\n", element->left->data, element->left->prev->data);
            fprintf(output, "  edge [color=\"#000000\"];\n");
        }

        print_graph_vertex_tree(element->left, output);
    }

    if (element->right != nullptr)
    {
        if (element->right->prev == element)
        {
            fprintf(output, "  \"%s\"->\"%s\";\n", element->data, element->right->data);
            fprintf(output, "  \"%s\"->\"%s\";\n", element->right->data, element->data);
        }
        else
        {
            fprintf(output, "  edge [color=\"#FE6200\"];\n");
            fprintf(output, "  \"%s\"->\"%s\";\n", element->data, element->right->data);
            fprintf(output, "  \"%s\"->\"%s\";\n", element->right->data, element->right->prev->data);
            fprintf(output, "  edge [color=\"#000000\"];\n");
        }

        print_graph_vertex_tree(element->right, output);
    }
}

const char *print_tree_error (int error)
{
    switch (error)
    {
        case TREE_REPEAT_INSERT_LEFT:
            return "REPEAT INSERT LEFT";
        case TREE_REPEAT_INSERT_RIGHT:
            return "REPEAT INSERT RIGHT";
        case TREE_INVALID_INSERT_ELEMENT:
            return "INVALID INSERT ELEMENT";
        case TREE_NULL_PTR:
            return "TREE ROOT NULL PTR";
        case TREE_ROOT_NULL_PTR:
            return "REPEAT INSERT RIGHT";
        case TREE_LINK_ERROR:
            return "LINK TREE ERROR";
        case TREE_INVALID_SIZE:
            return "INVALID TREE SIZE";
        default:
            return nullptr;
    }
}

