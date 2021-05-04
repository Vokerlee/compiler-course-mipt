#include "text.h"

void construct_text (text_t *text)
{
    assert(text);

    text->buffer   = nullptr;
    text->lines    = nullptr;
    text->counter  = nullptr;

    text->n_symbols    = 0;
    text->n_real_lines = 0;
    text->n_lines      = 0;
    text->line_counter = 0;
}

void destruct_text (text_t *text)
{
    assert(text);

    free(text->buffer);

    text->buffer   = nullptr;
    text->lines    = nullptr;
    text->counter  = nullptr;

    text->n_symbols    = -1;
    text->n_real_lines = -1;
    text->n_lines      = -1;
    text->line_counter = -1;
}

void fill_text (FILE *source, text_t *text, char read_regime)
{
    assert(source);
    assert(text);

    read_file(source, text, read_regime);

    text->lines = (line_t*) calloc(text->n_lines, sizeof(line_t));
    assert(text->lines);

    fill_text_lines(text, read_regime);
}

void read_file (FILE *source, text_t *text, char read_regime)
{
    assert(text);
    assert(source);

    find_length_of_file(source, text);

    text->buffer = (char *) calloc(text->n_symbols + 1, sizeof(char));
    assert(text->buffer);

    fread(text->buffer, sizeof(char), text->n_symbols, source);

    find_lines_of_file(text, read_regime);
}

void fill_text_lines (text_t *text, char read_regime)
{
    assert(text);

    text->counter = text->buffer;
    int real_num_line_counter = 1;

    for (size_t line_counter = 0; line_counter < text->n_real_lines; line_counter++)
    {
        if (read_regime == DEFAULT_REGIME)
        {
            while (isspace(*(text->counter)))
            {
                if (*(text->counter) == '\n')
                    real_num_line_counter++;

                text->counter++;
            }
        }
        else
        {
            while (isspace(*(text->counter)) || strncmp(text->counter, "//", 2) == 0)
            {
                if (isspace(*(text->counter)) && *(text->counter) == '\n')
                    real_num_line_counter++;
                else if (strncmp(text->counter, "//", 2) == 0)
                {
                    while (*(text->counter) != '\n')
                        text->counter++;
                }

                text->counter++;
            }
        }

        text->lines[line_counter].line = text->counter;
        text->lines[line_counter].real_num_line = real_num_line_counter;

        if (read_regime == DEFAULT_REGIME)
        {
            while (*(text->counter) != '\n' && *(text->counter) != '\0')
                text->counter++;

            text->lines[line_counter].length = text->counter - text->lines[line_counter].line;
        }
        else
        {
            while (*(text->counter) != '\n' && *(text->counter) != '\0' && strncmp(text->counter, "//", 2) != 0)
                text->counter++;

            char *end_of_line = text->counter - 1;

            while (isspace(*end_of_line))
                end_of_line--;

            text->lines[line_counter].length = (end_of_line - text->lines[line_counter].line) + 1;

            if (strncmp(text->counter, "//", 2))
            {
                while (*(text->counter) != '\n' && *(text->counter) != '\0')
                    text->counter++;
            }
        }
    }
}

void find_length_of_file (FILE *source, text_t *text)
{
    assert(source);

    fseek(source, 0, SEEK_END);

    text->n_symbols = ftell(source);

    fseek(source, 0, SEEK_SET);
}

void find_lines_of_file (text_t *text, char read_regime)
{
    assert(text);

    size_t counter = 0;

    while (counter < text->n_symbols)
    {
        if (text->buffer[counter] == '\n')
        {
            while (text->buffer[counter] == '\n')
            {
                text->n_lines++;
                counter++;

                while (isspace(text->buffer[counter]) && text->buffer[counter] != '\n' && text->buffer[counter] != '\0')
                    counter++;
            } 

            if (text->buffer[counter] == '\0')
                break;

            if (read_regime == NO_COMMENTS && strncmp(text->buffer + counter, "//", 2) == 0)
            {
                while (text->buffer[counter] != '\n')
                    counter++;

                counter--;
            }
            else
                text->n_real_lines++;
        }

        counter++;
    }

    text->n_lines++;
}

void print_text_lines (FILE *res, text_t *text)
{
    for (size_t line_counter = 0; line_counter < text->n_real_lines; line_counter++)
    {
        for (size_t i = 0; i < text->lines[line_counter].length; i++)
            fprintf (res, "%c", text->lines[line_counter].line[i]);

        fprintf(res, "\n");
    }
}

void current_time (char *time_line)
{
    time_t current_time = time(nullptr);
    tm *local_time = localtime(&current_time);

    if (local_time->tm_hour < 10)
        sprintf(time_line, "0%d:", local_time->tm_hour);
    else
        sprintf(time_line, "%d:", local_time->tm_hour);

    if (local_time->tm_min < 10)
        sprintf(time_line + 3, "0%d:", local_time->tm_min);
    else
        sprintf(time_line + 3, "%d:", local_time->tm_min);

    if (local_time->tm_sec < 10)
        sprintf(time_line + 6, "0%d", local_time->tm_sec);
    else
        sprintf(time_line + 6, "%d", local_time->tm_sec);

    sprintf(time_line + 8, ", ");

    if (local_time->tm_mday < 10)
        sprintf(time_line + 10, "0%d.", local_time->tm_mday);
    else
        sprintf(time_line + 10, "%d.", local_time->tm_mday);

    if (local_time->tm_mon < 10)
        sprintf(time_line + 13, "0%d.", local_time->tm_mon + 1);
    else
        sprintf(time_line + 13, "%d.", local_time->tm_mon + 1);

    sprintf(time_line + 16, "20%d", local_time->tm_year - 100);
}
