// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such log, much time!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <time.h>
#include "demeter.h"

static time_t slurm_time_to_time(char *slurm_time)
{
    struct tm timeinfo;
    int slurm_time_idx = 0;

    slurm_time_idx += get_len_to_char(&slurm_time[slurm_time_idx], '[');
    strptime(&slurm_time[slurm_time_idx], "[%Y-%m-%dT%H:%M:%S]", &timeinfo);
    return(mktime(&timeinfo));
}

static time_t sys_time_to_time(char *sys_time)
{
    struct tm timeinfo;

    strptime(sys_time, "%Y %b %d  %H:%M:%S", &timeinfo);
    return(mktime(&timeinfo));
}

int get_sys_log_time(parsed_log_t *log_to_parse, time_t start_time)
{
    char time_str[80];
    time_t log_time = 0;
    struct tm *timeinfo;
    bool is_timestamp = true;

    memset(time_str, 0, 80);
    if (!log_to_parse->unparsed_log)
        return (0);
    for (int i = 0; i < 10 && log_to_parse->unparsed_log[i] != 0; i++) {
        if (log_to_parse->unparsed_log[i] < '0' || log_to_parse->unparsed_log[i] > '9')
            is_timestamp = false;
    }
    if (is_timestamp) {
        log_time = (time_t)atol((log_to_parse->unparsed_log));
        if (log_time < start_time)
            return(3);
        timeinfo = localtime(&log_time);
    } else {
        log_time = sys_time_to_time(log_to_parse->unparsed_log);
        if (log_time < start_time)
            return(3);
        timeinfo = localtime(&log_time);
    }
    strftime(time_str, 80, "[%Y-%m-%dT%H:%M:%S]", timeinfo);
    log_to_parse->log_time_str = strdup(time_str);
    return (0);
}

int get_slurm_log_time(parsed_log_t *log_to_parse, time_t start_time)
{
    char *time_str = NULL;
    int len = 0;

    if (!log_to_parse->log_source_path || strcmp(log_to_parse->log_source_path, "slurm_log_path") != 0)
        return(1);
    for (len = 0; log_to_parse->unparsed_log[len] != '\0' && log_to_parse->unparsed_log[len] != ']'; len++);
    if (log_to_parse->unparsed_log[len] == '\0')
        return(1);
    time_str = strndup(log_to_parse->unparsed_log, len + 1);
    if (slurm_time_to_time(time_str) < start_time) {
        free(time_str);
        return(2);
    }
    log_to_parse->log_time_str = time_str;
    return (0);
}