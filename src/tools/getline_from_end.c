// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such read, good file, much end!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>

ssize_t getline_from_end(char **line, size_t *len, FILE *file, long *line_offset)
{
    long line_count = 0;
    ssize_t return_value = 0;
    int char_return_value = 0;

    fseek(file, 0, SEEK_END);
    while (line_count <= *line_offset) {
        fseek(file, -2, SEEK_CUR);
        char_return_value = fgetc(file);
        if (char_return_value == '\n')
            line_count++;
    }
    return_value = getline(line, len, file);
    return (return_value);
}