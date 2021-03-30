int length_of_file(FILE *onegin)
{
    assert(onegin);

    fseek(onegin, 0, SEEK_END);
    int length_file = ftell(onegin);
    fseek(onegin, 0, SEEK_SET);

    return length_file;
}

void lines_of_file(char *stream, size_t *lines_file, size_t *length_file)
{
    assert(stream);
    assert(lines_file);

    for (size_t i = 0; i < *length_file; i++)
        if (stream[i] == '\n')
            (*lines_file)++;

    (*lines_file)++;
}

char *read_file (FILE *onegin, size_t *length_file, size_t *lines_file)
{
    assert(onegin);
    assert(length_file);
    assert(lines_file);

    *length_file = length_of_file(onegin);

    char *stream = (char *) calloc(*length_file, sizeof(char)); // массив хранения символов файла
    assert(stream);

    fread(stream, sizeof(char), *length_file, onegin);

    lines_of_file(stream, lines_file, length_file);

    return stream;
}

void write_in_structs (line_t *lines, char *stream, size_t *n_symb, size_t *n_struct, size_t *file_length)
{
    assert(lines);
    assert(stream);
    assert(n_symb);
    assert(n_struct);
    assert(file_length);

    while (TRUE) // читаем строки из массива stream в структуры
    {
        while(TRUE)
        {
            if (*n_symb >= *file_length)
                break;
            if (isspace(stream[*n_symb]))
                (*n_symb)++;
            else
                break;
        }

        if (*n_symb >= *file_length)
            break;

        lines[*n_struct].line = &stream[*n_symb];
        lines[*n_struct].length = *n_symb;

        while(stream[*n_symb] != '\n')
        {
            if (*n_symb >= *file_length)
                break;
            (*n_symb)++;
        }

        lines[*n_struct].length = *n_symb - lines[*n_struct].length;
        (*n_symb)++;
        (*n_struct)++;
    }
}

int (*choose_comp(char *regime))(const void *, const void *)
{
    assert(regime);

    if (!strcmp(regime, "rhyme"))
        return strcomp_rhyme;
    else if (!strcmp(regime, "descending"))
        return strcomp_descen;
    else
        return strcomp_ascen;
}

void print_structs(FILE *res, line_t *lines, size_t n_struct)
{
    for (size_t num_struct = 0; num_struct < n_struct; ++num_struct)
        for (size_t i = 0; i < lines[num_struct].length; i++)
            fprintf (res, "%c", lines[num_struct].line[i]);
}

void sortlines (FILE *onegin, FILE *res, char *regime, char *bred_ind)
{
    assert(onegin);
    assert(res);
    assert(regime);
    assert(bred_ind);

    size_t n_symb      = 0;
    size_t n_struct    = 0;
    size_t file_length = 0;
    size_t file_lines  = 0;

    char *stream = read_file(onegin, &file_length, &file_lines);
    assert(stream);

    line_t *lines = (line_t*) calloc(file_lines, sizeof(line_t)); // инициализируем структуры
    assert(lines);

    write_in_structs(lines, stream, &n_symb, &n_struct, &file_length);

    int (*str_comp)(const void*, const void *) = choose_comp(regime); // выбираем компаратор по режиму

    megasort(lines, n_struct, str_comp);

    bred_generator(lines, bred_ind, n_struct);

    print_structs(res, lines, n_struct);

    free(stream);
}
