// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, much compressed!
//___________________________________________________________________________________________________________________________________________

#define _GNU_SOURCE
#include <stdio.h>
#include "demeter.h"

static time_t get_rotate_time(char *file_name)
{
    time_t rotate_time = 0;
    char *rotate_time_str = NULL;
    struct tm rotate_time_tm;

    if (file_name == NULL)
        return (0);
    rotate_time_str = strstr(file_name, "log-");
    if (rotate_time_str == NULL)
        return (0);
    rotate_time_str += 4;
    rotate_time_tm.tm_year = atoi(strndup(rotate_time_str, 4)) - 1900;
    rotate_time_tm.tm_mon = atoi(strndup(rotate_time_str + 4, 2)) - 1;
    rotate_time_tm.tm_mday = atoi(strndup(rotate_time_str + 6, 2)) + 1;
    rotate_time_tm.tm_hour = 0;
    rotate_time_tm.tm_min = 0;
    rotate_time_tm.tm_sec = 0;
    rotate_time = mktime(&rotate_time_tm);
    return (rotate_time);
}

FILE *open_slurm_log(demeter_conf_t *demeter_conf)
{
    char *log_path = NULL, *hostname = NULL;
    FILE *log_file = NULL;

    if ((hostname = get_hostname()) == NULL)
        return (NULL);
    asprintf(&log_path, "%sslurm.%s.log", demeter_conf->slurm_log_path, hostname);
    if (log_path == NULL) {
        write_log_to_file(demeter_conf, "Cannot get slurm log path", DEBUG, 99);
        free(hostname);
        return (NULL);
    }
    if ((log_file = fopen(log_path, "r")) == NULL) {
        write_log_to_file(demeter_conf, "Cannot open slurm log file", DEBUG, 99);
        free(log_path);
        free(hostname);
        return (NULL);
    }
    free(log_path);
    free(hostname);
    return (log_file);
}

gzFile open_rotated_slurm_log(demeter_conf_t *demeter_conf, job_id_info_t *job_info)
{
    char *log_path = NULL, *hostname = NULL;
    struct dirent *entry = NULL;
    gzFile log_file = NULL;
    DIR *dir = NULL;
    time_t rotate_time = 0;

    if ((hostname = get_hostname()) == NULL)
        return (NULL);
    if ((dir = opendir(demeter_conf->slurm_log_path)) == NULL) {
        write_log_to_file(demeter_conf, "Cannot open slurm log directory", INFO, 0);
        free(hostname);
        return (NULL);
    }
    while (log_path == NULL) {
        if ((entry = readdir(dir)) == NULL) {
            write_log_to_file(demeter_conf, "Cannot find rotated slurm log file",  INFO, 0);
            free(hostname);
            closedir(dir);
            return (NULL);
        }
        if (strstr(entry->d_name, hostname) != NULL && strstr(entry->d_name, ".gz") != NULL) {
            rotate_time = get_rotate_time(entry->d_name);
            if (rotate_time == -1) {
                write_log_to_file(demeter_conf, "Cannot get rotated slurm log file time", INFO, 0);
                free(hostname);
                closedir(dir);
                return (NULL);
            }
            if (rotate_time > job_info->start_time)
                asprintf(&log_path, "%s%s", demeter_conf->slurm_log_path, entry->d_name);
            else 
                write_log_to_file(demeter_conf, "wrong time for file", INFO, 0);
        }
    }
    closedir(dir);
    log_file = gzopen(log_path, "r");
    free(log_path);
    free(hostname);
    if (log_file == NULL)
        return (NULL);
    return (log_file);
}