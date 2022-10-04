// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such read, good file, much end!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <zlib.h>
#include "demeter.h"

ssize_t getline_from_end(char **line, size_t *len, FILE *file, long *line_offset)
{
    long line_count = 0;
    ssize_t return_value = 0;
    int read_char = 0;

    fseek(file, 0, SEEK_END);
    while (line_count <= *line_offset) {
        if (fseek(file, -2, SEEK_CUR) == -1)
            return (-1);
        read_char = fgetc(file);
        if (read_char == '\n' || read_char == '\0')
            line_count++;
    }
    return_value = getline(line, len, file);
    if (return_value == -1) {
        free((*line));
        (*line) = NULL;
        return (-1);
    }
    return (return_value);
}

ssize_t zgetline_from_end(char **line, size_t *len, gzFile file, long *line_offset)
{
    long line_count = 0;
    ssize_t return_value = 0;
    int read_char = 0;
    bool reach_start = false;

    while ((read_char = gzgetc(file)) != EOF && read_char != '\0');
    while (file->pos > 0 && line_count <= *line_offset) {
        if (gzseek(file, -2, SEEK_CUR) == -1) {
            reach_start = true;
            break;
        }
        read_char = gzgetc(file);
        if (read_char == '\n' || read_char == '\0')
            line_count++;
    }
    if (reach_start) {
        gzseek(file, 0, SEEK_SET);
        zgetline(line, file);
        return_value = -42;
    } else
        return_value = zgetline(line, file);
    if ((*line) == NULL)
        return (-1);
    if (return_value > 0) {
        if (return_value == 1)
            free((*line));
        (*line) = NULL;
        return (-1);
    }
    return (return_value);
}