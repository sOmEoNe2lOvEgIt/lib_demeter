// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such get, good line, much compressed!
//___________________________________________________________________________________________________________________________________________

#include <zlib.h>
#include <stdlib.h>

int zgetline(char **line, gzFile file)
{
    int size = 0;
    int read_char = 0;

    (*line) = realloc((*line), sizeof(char));
    if (!(*line))
        return (2);
    for (;read_char != '\n' && (read_char = gzgetc(file)) != -1; size++) {
        (*line) = realloc((*line), size + 1);
        if (!(*line))
            return (2);
        (*line)[size] = read_char;
    }
    if (size == 0)
        return (1);
    (*line) = realloc((*line), size + 1);
    if (!(*line))
        return (2);
    (*line)[size] = '\0';
    return (0);
}
