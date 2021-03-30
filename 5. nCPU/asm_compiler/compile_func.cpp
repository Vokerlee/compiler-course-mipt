#include "compile.h"
#include "compiler_config.h"

void compile (text_t *text, FILE *res)
{
    FILE *log          = fopen("log_compile.txt", "wb");
    FILE *file_listing = fopen("file_listing.txt", "wb");

    assert(res);
    assert(log);
    assert(file_listing);
    assert(text);

    print_file_listing_header(file_listing);

    label labels = {};

    char *asm_buffer = (char *) calloc((sizeof(int) + sizeof(double)) * text->n_real_lines, sizeof(char));
    int bytes_buffer = 0;

    labels.numbers   = (int *)   calloc(text->n_real_lines, sizeof(int));
    labels.name_lab  = (char **) calloc(text->n_real_lines, sizeof(char *));
    labels.n_labels  = 0;
    labels.counter   = 0;

    for (size_t i = 0; i < text->n_real_lines; i++)
        fill_labels(&labels, &(text->lines[i]));

    for (size_t i = 0; i < text->n_real_lines; i++)
    {
        fprintf(file_listing, "%08x: ", bytes_buffer);

        int error_state = process_line(asm_buffer, &labels, &bytes_buffer, &(text->lines[i]), file_listing);

        if (error_state == ERROR_FORMAT)
            fprintf(log, "ERROR FORMATTING OF CODE IN COMMAND %Iu\n", text->lines[i].real_num_line);
        else if (error_state == ERROR_UNKNOWN_CMD)
            fprintf(log, "UNKNOWN COMMAND %Iu\n", text->lines[i].real_num_line);
    }

    write_id_asm(res);

    fwrite(&bytes_buffer, sizeof(int), 1, res);
    fwrite(asm_buffer, sizeof(char), bytes_buffer, res);

    fwrite(&(labels.n_labels), sizeof(int), 1, res);

    if (labels.n_labels != 0)
        fwrite(labels.numbers, sizeof(int), labels.n_labels, res);

    free(asm_buffer);
    free(labels.numbers);

    for (int i = 0; i < labels.n_labels; i++)
        free(labels.name_lab[i]);

    free(labels.name_lab);
}

void print_file_listing_header (FILE *file_listing)
{
    assert(file_listing);

    fprintf(file_listing, "File-listing:\n"
                          "code %*.c ->", FILE_LISTING_INDENT / 2, ' ');

    fprintf(file_listing, "%*.c compiled_code\n", FILE_LISTING_INDENT / 2 + FILE_LISTING_INDENT % 2 + 2, ' ');
}

void fill_labels (label *labels, line_t *line)
{
    assert(labels);
    assert(line);

    if (line->line[line->length - 1] == ':')
    {
        labels->name_lab[labels->n_labels] = (char *) calloc(line->length, sizeof(char));

        strncpy(labels->name_lab[labels->n_labels], line->line, line->length - 1);

        labels->n_labels++;
    }
}

int process_line (char *asm_buffer, label *labels, int *bytes_buffer, line_t *line, FILE *file_listing)
{
    assert(asm_buffer);
    assert(labels);
    assert(bytes_buffer);
    assert(line);
    assert(file_listing);

    char *command = (char *) calloc(line->length, sizeof(char));
    char *label   = (char *) calloc(line->length, sizeof(char));
    char regist   = 0;
    double number = NAN;

    int read_state = read_command(line, command, label, &regist, &number);

    if (read_state == ERROR_READ)
        return ERROR_FORMAT;
    else if (read_state == READ_LABEL)
        process_label_line(labels, bytes_buffer, file_listing);
    else
    {
        int command_number = hash_search(hash_text(command));

        if (command_number == ERROR_HASH)
            return ERROR_UNKNOWN_CMD;
        else if (regist != 0 && !isnan(number) && asm_data[command_number] != PUSH && asm_data[command_number] != POP && asm_data[command_number] != IN)
            return ERROR_UNKNOWN_CMD;
        else if (read_state == READ_CMD_LABEL)
            process_label_cmd_line(asm_buffer, bytes_buffer, command, label, labels, command_number, file_listing);
        else if (read_state == RAM_CMD || read_state == RAM_CMD_NUM)
            process_ram_line(asm_buffer, bytes_buffer, command, command_number, number, regist, file_listing);
        else if (read_state == NO_ARGUMENT_CMD)
            process_no_argument_cmd_line(asm_buffer, bytes_buffer, command, command_number, regist, file_listing);
        else if (read_state == ARGUMENT_CMD)
            process_argument_cmd_line(asm_buffer, bytes_buffer, command, command_number, number, file_listing);
    }

    free(command);
    free(label);

    return 0;
}

int read_command (line_t *line, char *command, char *label_name, char *regist, double *number)
{
    assert(line);
    assert(label_name);
    assert(command);
    assert(number);

    int number_sign = 1;

    int counter = 0;

    if (isdigit(line->line[counter]))
        return ERROR_READ;

    if (line->line[line->length - 1] == ':')
        return READ_LABEL;

    for (counter = 0; !isspace(line->line[counter]) && counter < line->length; counter++)
        command[counter] = line->line[counter];

    while (line->line[counter] == ' ' || line->line[counter] == '\t')
        counter++;

    if (isdigit(line->line[counter]) || line->line[counter] == '-')
    {
        *number = atof(&(line->line[counter]));

        return ARGUMENT_CMD;
    }

    if (line->line[counter] == ':')
    {
        strncpy(label_name, line->line + counter + 1, line->length - counter - 1);

        return READ_CMD_LABEL;
    }

    if (line->line[counter] == '[')
    {
        counter++;

        while (line->line[counter] == ' ' || line->line[counter] == '\t')
            counter++;

        *number = 0;

        if (isdigit(line->line[counter]))
        {
            *number = atof(&(line->line[counter]));

            while (isdigit(line->line[counter]))
                counter++;

            while (line->line[counter] == ' ' || line->line[counter] == '\t')
                counter++;

            if (line->line[counter] != ']')
                return ERROR_READ;

            return RAM_CMD_NUM;
        }
        else if (isalpha(line->line[counter]))
        {
            if (line->line[counter] == 'r' && line->line[counter + 1] >= 'a' && line->line[counter + 1] <= 'e' && line->line[counter + 2] == 'x')
                 *regist = line->line[counter + 1] - 'a' + 1;
            else
                return ERROR_READ;

            counter += 3;

            while (line->line[counter] == ' ' || line->line[counter] == '\t')
                counter++;

            if (line->line[counter] == '+')
                counter++;
            else if (line->line[counter] == '-')
            {
                number_sign = -1;
                counter++;
            }

            while (line->line[counter] == ' ' || line->line[counter] == '\t')
                counter++;

            if (isdigit(line->line[counter]))
                *number = number_sign * atof(&(line->line[counter]));

            while (isdigit(line->line[counter]) || line->line[counter] == '-')
                counter++;

            while (line->line[counter] == ' ' || line->line[counter] == '\t')
                counter++;

            if (line->line[counter] != ']')
                return ERROR_READ;
        }

        return RAM_CMD;
    }

    if (isalpha(line->line[counter]))
    {
        if (line->line[counter] == 'r' && line->line[counter + 1] >= 'a' && line->line[counter + 1] <= 'e' && line->line[counter + 2] == 'x')
            *regist = line->line[counter + 1] - 'a' + 1;
    }

    return NO_ARGUMENT_CMD;
}

void process_label_line (label *labels, int *bytes_buffer, FILE *file_listing)
{
    assert(labels);
    assert(bytes_buffer);
    assert(file_listing);

    labels->numbers[labels->counter] = *bytes_buffer;

    int alignment_var = fprintf(file_listing, "%s:", labels->name_lab[labels->counter]);

    fprintf(file_listing, "%*.c", FILE_LISTING_INDENT - alignment_var + 1, ' ');

    fprintf(file_listing, "\"");
    hex_print((unsigned char *) &(labels->numbers[labels->counter]), sizeof(int), file_listing);
    fprintf(file_listing, "\"\n");

    labels->counter += 1;
}

void process_label_cmd_line (char *asm_buffer, int *bytes_buffer, char *command, char *label_name, label *labels, int command_number, FILE *file_listing)
{
    assert(asm_buffer);
    assert(bytes_buffer);
    assert(command);
    assert(labels);
    assert(label_name);
    assert(file_listing);

    asm_buffer[*bytes_buffer] = asm_data[command_number];
    *bytes_buffer += sizeof(char);

    int counter = 0;

    for (counter = 1; counter <= labels->n_labels; counter++)
    {
        if (strcmp(label_name, labels->name_lab[counter - 1]) == 0)
        {
            *((int *) (asm_buffer + *bytes_buffer)) = counter;
            break;
        }
    }

    *bytes_buffer += sizeof(int);

    int alignment_var = fprintf(file_listing, "%s :%-10s", command, labels->name_lab[counter - 1]);

    fprintf(file_listing, "%*.c", FILE_LISTING_INDENT - alignment_var + 1, ' ');

    fprintf(file_listing, "\"");
    hex_print((unsigned char *) &asm_data[command_number], sizeof(char), file_listing);
    fprintf(file_listing, " ");

    hex_print((unsigned char *) &counter, sizeof(int), file_listing);
    fprintf(file_listing, "\"\n");
}

void process_no_argument_cmd_line (char *asm_buffer, int *bytes_buffer, char *command, int command_number, int regist, FILE *file_listing)
{
    assert(asm_buffer);
    assert(bytes_buffer);
    assert(command);
    assert(file_listing);

    char code_command = asm_data[command_number] + regist;

    asm_buffer[*bytes_buffer] = code_command;
    *bytes_buffer += sizeof(char);

    int alignment_var = 0;

    if (regist != 0)
        alignment_var = fprintf(file_listing, "%s r%cx", command, 'a' + regist - 1);
    else
        alignment_var = fprintf(file_listing, "%s", command);

    fprintf(file_listing, "%*.c", FILE_LISTING_INDENT - alignment_var + 1, ' ');

    fprintf(file_listing, "\"");
    hex_print((unsigned char *) &code_command, sizeof(char), file_listing);
    fprintf(file_listing, "\"\n");
}

void process_argument_cmd_line (char *asm_buffer, int *bytes_buffer, char *command, int command_number, double number, FILE *file_listing)
{
    assert(asm_buffer);
    assert(bytes_buffer);
    assert(command);
    assert(file_listing);

    int alignment_var = 0;

    asm_buffer[*bytes_buffer] = asm_data[command_number];
    *bytes_buffer += sizeof(char);

    *((double *) (asm_buffer + *bytes_buffer)) = number;
    *bytes_buffer += sizeof(double);

    alignment_var = fprintf(file_listing, "%s %-15lg", command, number);

    fprintf(file_listing, "%*.c", FILE_LISTING_INDENT - alignment_var + 1, ' ');

    fprintf(file_listing, "\"");
    hex_print((unsigned char *) &asm_data[command_number], sizeof(char), file_listing);
    fprintf(file_listing, " ");

    hex_print((unsigned char *) &number, sizeof(double), file_listing);

    fprintf(file_listing, "\"\n");
}

void process_ram_line (char *asm_buffer, int *bytes_buffer, char *command, int command_number, double number, char regist, FILE *file_listing)
{
    assert(asm_buffer);
    assert(bytes_buffer);
    assert(command);
    assert(file_listing);

    int alignment_var = 0;

    char code_command = asm_data[command_number] + 30 + regist;

    asm_buffer[*bytes_buffer] = code_command;
    *bytes_buffer += sizeof(char);

    int index_ram = (int) number;

    *((int *) (asm_buffer + *bytes_buffer)) = index_ram;
    *bytes_buffer += sizeof(int);

    if (regist != 0)
    {
        if (index_ram > 0)
            alignment_var = fprintf(file_listing, "%s [r%cx + %d]", command, 'a' + regist - 1, index_ram);
        else if (index_ram < 0)
            alignment_var = fprintf(file_listing, "%s [r%cx - %d]", command, 'a' + regist - 1, -index_ram);
        else
            alignment_var = fprintf(file_listing, "%s [r%cx]", command, 'a' + regist - 1);
    }
    else
        alignment_var = fprintf(file_listing, "%s [%d]", command, index_ram);

    fprintf(file_listing, "%*.c", FILE_LISTING_INDENT - alignment_var + 1, ' ');

    fprintf(file_listing, "\"");
    hex_print((unsigned char *) &code_command, sizeof(char), file_listing);
    fprintf(file_listing, " ");
    hex_print((unsigned char *) &index_ram, sizeof(int), file_listing);
    fprintf(file_listing, "\"\n");
}

void write_id_asm (FILE *res)
{
    assert(res);

    char ID[4] = {"ID6"};

    fwrite(&ID, sizeof(char), 3, res);
}

int hash_text (const char *line)
{
    assert(line);

    int temp = 1;

    int length_of_line = strlen(line);

    for (int i = 1; i <= length_of_line; i++)
        temp += temp * i * i * line[i - 1];

    return temp;
}

int hash_search (int hash_command)
{
    for (int i = 0; i < DATA_SIZE; i++)
        if (hash_command == hash_data[i])
            return i;

    return ERROR_HASH;
}

void hex_print (unsigned char *hex, size_t size_hex, FILE *res)
{
    assert(hex);
    assert(res);

    for (int i = size_hex - 1; i >= 1; i--)
        fprintf(res, "%02x ", hex[i]);

    fprintf(res, "%02x", hex[0]);
}


