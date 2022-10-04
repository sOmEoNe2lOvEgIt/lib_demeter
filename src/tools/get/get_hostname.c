// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such host, much name!
//___________________________________________________________________________________________________________________________________________


#include <stdio.h>
#include "demeter.h"

char *get_hostname(void)
{
    char *hostname = NULL;
    char *line = NULL;
    FILE *hostname_file = NULL;
    size_t hostname_len = 100;

    if ((hostname_file = fopen("/etc/hostname", "r")) == NULL)
        return (NULL);
    getline(&line, &hostname_len, hostname_file);
    fclose(hostname_file);
    hostname_len = get_len_to_char(line, '\n');
    line[hostname_len] = '\0';
    hostname = strdup(line);
    if (line != NULL)
        free(line);
    else
        return (NULL);
    return (hostname);
}