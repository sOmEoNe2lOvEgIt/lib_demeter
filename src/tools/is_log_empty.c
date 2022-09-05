// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, much test!
//___________________________________________________________________________________________________________________________________________

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool is_log_empty(char *log)
{
    if (log == NULL)
        return (true);
    if (strlen(log) == 0) {
        free(log);
        return (true);
    }
    if (strcmp(log, "\n") == 0) {
        free(log);
        return (true);
    }
    return (false);
}
