// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such log, much time!
//___________________________________________________________________________________________________________________________________________

#include <time.h>
#include <stdio.h>
#include "demeter.h"

int get_sys_log_time(parsed_log_t *log_to_parse, time_t start_time)
{
    FILE *uptime_file = NULL;
    char *uptime_str = NULL, time_str[80];
    time_t uptime = 0, log_time = 0;
    size_t uptime_str_len = 30;
    int len_before_timestamp = 0;
    struct tm *timeinfo;

    memset(time_str, 0, 80);
    if (log_to_parse->log_source_path == NULL || strcmp(log_to_parse->log_source_path, "syslog") != 0)
        return(1);
    len_before_timestamp = get_len_to_char(log_to_parse->unparsed_log, '[');
    if (log_to_parse->unparsed_log[len_before_timestamp] == '\0')
        return(1);
    len_before_timestamp ++;
    uptime_file = fopen("/proc/uptime", "r");
    if (uptime_file == NULL)
        return(2);
    getline(&uptime_str, &uptime_str_len, uptime_file);
    fclose(uptime_file);
    uptime = (time_t)atol(uptime_str);
    log_time = (time_t)atol((log_to_parse->unparsed_log + len_before_timestamp));
    log_time += (time(NULL) - uptime); //boot time + time since boot to log
    if (log_time < start_time) {
        free (uptime_str);
        return(3);
    }
    timeinfo = localtime(&log_time);
    sprintf(time_str, "[%d-%02d-%02dT%02d:%02d:%02d]", (timeinfo->tm_year + 1900), (timeinfo->tm_mon + 1), timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    log_to_parse->log_time_str = strdup(time_str);    
    free(uptime_str);
    return (0);
}

time_t slurm_time_to_time(char *slurm_time)
{
    struct tm timeinfo;
    int slurm_time_idx = 0;

    if (slurm_time == NULL)
        return (0);
    memset(&timeinfo, 0, sizeof(struct tm));
    slurm_time_idx += get_len_to_char(&slurm_time[slurm_time_idx], '[') + 1;
    timeinfo.tm_year = atoi(&slurm_time[slurm_time_idx]) - 1900;
    slurm_time_idx += get_len_to_char(&slurm_time[slurm_time_idx], '-') + 1;
    timeinfo.tm_mon = atoi(&slurm_time[slurm_time_idx]) - 1;
    slurm_time_idx += get_len_to_char(&slurm_time[slurm_time_idx], '-') + 1;
    timeinfo.tm_mday = atoi(&slurm_time[slurm_time_idx]);
    slurm_time_idx += get_len_to_char(&slurm_time[slurm_time_idx], 'T') + 1;
    timeinfo.tm_hour = atoi(&slurm_time[slurm_time_idx]);
    slurm_time_idx += get_len_to_char(&slurm_time[slurm_time_idx], ':') + 1;
    timeinfo.tm_min = atoi(&slurm_time[slurm_time_idx]);
    slurm_time_idx += get_len_to_char(&slurm_time[slurm_time_idx], ':') + 1;
    timeinfo.tm_sec = atoi(&slurm_time[slurm_time_idx]);
    return(mktime(&timeinfo));
}

int get_slurm_log_time(parsed_log_t *log_to_parse, time_t start_time)
{
    char *time_str = NULL;
    int len = 0;

    if (log_to_parse->log_source_path == NULL || strcmp(log_to_parse->log_source_path, "slurm_log_path") != 0)
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