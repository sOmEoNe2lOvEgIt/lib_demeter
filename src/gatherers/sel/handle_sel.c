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

int handle_sel_time(parsed_sel_t *curr_sel, demeter_conf_t *demeter_conf, time_t start_time)
{
    int i = 0;
    struct tm *sel_time = malloc(sizeof(struct tm));
    char time_str[80];

    for (; curr_sel->unparsed_sel[i] != '|' && curr_sel->unparsed_sel[i] != '\0'; i++);
    if (curr_sel->unparsed_sel[i] == '\0' && curr_sel->unparsed_sel[i + 2] == '\0')
        return (1);
    i += 2;
    sel_time->tm_mday = atoi(&curr_sel->unparsed_sel[i]);
    for (; curr_sel->unparsed_sel[i] != '/' && curr_sel->unparsed_sel[i] != '\0'; i++);
    if (curr_sel->unparsed_sel[i] == '\0' && curr_sel->unparsed_sel[i + 1] == '\0')
        return (1);
    i++;
    sel_time->tm_mon = atoi(&curr_sel->unparsed_sel[i]);
    for (; curr_sel->unparsed_sel[i] != '/' && curr_sel->unparsed_sel[i] != '\0'; i++);
    if (curr_sel->unparsed_sel[i] == '\0' && curr_sel->unparsed_sel[i + 1] == '\0')
        return (1);
    i++;
    sel_time->tm_year = atoi(&curr_sel->unparsed_sel[i]);
    for (; curr_sel->unparsed_sel[i] != '|' && curr_sel->unparsed_sel[i] != '\0'; i++);
    if (curr_sel->unparsed_sel[i] == '\0' && curr_sel->unparsed_sel[i + 2] == '\0')
        return (1);
    i += 2;
    sel_time->tm_hour = atoi(&curr_sel->unparsed_sel[i]);
    for (; curr_sel->unparsed_sel[i] != ':' && curr_sel->unparsed_sel[i] != '\0'; i++);
    if (curr_sel->unparsed_sel[i] == '\0' && curr_sel->unparsed_sel[i + 1] == '\0')
        return (1);
    i++;
    sel_time->tm_min = atoi(&curr_sel->unparsed_sel[i]);
    for (; curr_sel->unparsed_sel[i] != ':' && curr_sel->unparsed_sel[i] != '\0'; i++);
    if (curr_sel->unparsed_sel[i] == '\0' && curr_sel->unparsed_sel[i + 1] == '\0')
        return (1);
    i++;
    sel_time->tm_sec = atoi(&curr_sel->unparsed_sel[i]);
    sprintf(time_str, "[%d-%02d-%02dT%02d:%02d:%02d]", sel_time->tm_year, sel_time->tm_mon, sel_time->tm_mday, sel_time->tm_hour, sel_time->tm_min, sel_time->tm_sec);
    curr_sel->sel_time_str = strdup(time_str);
    free(sel_time);
    return (0);
}

int handle_sel_type(parsed_sel_t *curr_sel)
{
    int i = 0;
    int len = 0;

    for (int j = 0; j < 2; j++)
        for (; curr_sel->unparsed_sel[i] != '|' && curr_sel->unparsed_sel[i] != '\0'; i++);
    if (curr_sel->unparsed_sel[i] == '\0')
        return (1);
    i++;
    for (; curr_sel->unparsed_sel[i] != '|' && curr_sel->unparsed_sel[i] != '\0'; i++, len++);
    len --;
    curr_sel->sel_msg_type = strndup(&curr_sel->unparsed_sel[i - len], len);
    return (0);
}

int handle_sel_msg(parsed_sel_t *curr_sel)
{
    int i = 0;
    int len = 0;

    for (int j = 0; j < 3; j++)
        for (; curr_sel->unparsed_sel[i] != '|' && curr_sel->unparsed_sel[i] != '\0'; i++);
    if (curr_sel->unparsed_sel[i] == '\0')
        return (1);
    i++;
    for (; curr_sel->unparsed_sel[i] != '|' && curr_sel->unparsed_sel[i] != '\0'; i++, len++);
    len --;
    curr_sel->sel_msg = strndup(&curr_sel->unparsed_sel[i - len], len);
    return (0);
}

int handle_sel_assert(parsed_sel_t *curr_sel)
{
    int i = 0;
    int len = 0;

    for (int j = 0; j < 4; j++)
        for (; curr_sel->unparsed_sel[i] != '|' && curr_sel->unparsed_sel[i] != '\0'; i++);
    if (curr_sel->unparsed_sel[i] == '\0')
        return (1);
    i += 2;
    for (; curr_sel->unparsed_sel[i] != '\0'; i++, len++);
    if (strstr(&curr_sel->unparsed_sel[i - len], "Asserted") != NULL)
        curr_sel->asserted = true;
    else
        curr_sel->asserted = false;
    return (0);
}