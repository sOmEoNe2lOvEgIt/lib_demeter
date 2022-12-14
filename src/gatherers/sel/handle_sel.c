// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such sel, many anger, great logs!
// (this is a play on words as sel is french for salt as in salty so angry...)
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <time.h>
#include "slurm/slurm.h"
#include "demeter.h"

int get_sel_assert(parsed_sel_t *curr_sel)
{
    int i = 0, j = 0;

    for (; j < 5; j++)
        i += get_len_to_char(&curr_sel->unparsed_sel[i], '|') + 1;
    i++;
    if (curr_sel->unparsed_sel[i] == '\0')
        return (1);
    if (strncmp(&curr_sel->unparsed_sel[i], "Asserted", 8) == 0)
        curr_sel->asserted = true;
    else
        curr_sel->asserted = false;
    return (0);
}

int get_sel_element(parsed_sel_t *curr_sel, char **element, int element_nb)
{
    int i = 0, len = 0;

    for (; element_nb > 0 ; element_nb--)
        i += get_len_to_char(&curr_sel->unparsed_sel[i], '|') + 1;
    i++;
    if (curr_sel->unparsed_sel[i] == '\0')
        return (1);
    for (; curr_sel->unparsed_sel[i] != '|' && curr_sel->unparsed_sel[i] != '\0'; i++, len++);
    (*element) = strndup(&curr_sel->unparsed_sel[i - len], len);
    return (0);
}

int get_sel_time(parsed_sel_t *curr_sel, time_t start_time)
{
    int i = 0;
    struct tm *sel_time = malloc(sizeof(struct tm));
    char time_str[80];

    memset(time_str, 0, 80);
    i += get_len_to_char(&curr_sel->unparsed_sel[i], '|');
    if (curr_sel->unparsed_sel[i] == '\0')
        return (1);
    sscanf(&curr_sel->unparsed_sel[i], "| %d/%d/%d | %d:%d:%d |",
    &sel_time->tm_mday, &sel_time->tm_mon, &sel_time->tm_year,
    &sel_time->tm_hour, &sel_time->tm_min, &sel_time->tm_sec);
    if (mktime(sel_time) < start_time)
        return (1);
    sprintf(time_str, "[%d-%02d-%02dT%02d:%02d:%02d]",
    sel_time->tm_year, sel_time->tm_mon, sel_time->tm_mday,
    sel_time->tm_hour, sel_time->tm_min, sel_time->tm_sec);
    curr_sel->sel_time_str = strdup(time_str);
    free(sel_time);
    return (0);
}