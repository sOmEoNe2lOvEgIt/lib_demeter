// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such append, much str!
//___________________________________________________________________________________________________________________________________________

#include <stdlib.h>
#include <string.h>

char *append_str(char *str, char *to_append)
{
    uint len;

    if (!to_append)
        return (str);
    if (!str)
        return (to_append);
    len = strlen(str) + strlen(to_append) + 1;
    if (len < 0)
        return (NULL);
    str = realloc(str, len * sizeof(char));
    if (!str)
        return (NULL);
    strcat(str, to_append);
    return(str);
}