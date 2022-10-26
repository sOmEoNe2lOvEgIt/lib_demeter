// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, much compressed!
//___________________________________________________________________________________________________________________________________________

#define _GNU_SOURCE
#include <stdio.h>
#include "demeter.h"

FILE *open_slurm_log(demeter_conf_t *demeter_conf)
{
    char *log_path = NULL, *hostname = NULL;
    FILE *log_file = NULL;

    if (!(hostname = get_hostname()))
        return (NULL);
    asprintf(&log_path, "%sslurm.%s.log", demeter_conf->slurm_log_path, hostname);
    if (!log_path) {
        write_log_to_file(demeter_conf, "Cannot get slurm log path", DEBUG, 99);
        free(hostname);
        return (NULL);
    }
    if (!(log_file = fopen(log_path, "r"))) {
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

    if (!(hostname = get_hostname()))
        return (NULL);
    if (!(dir = opendir(demeter_conf->slurm_log_path))) {
        write_log_to_file(demeter_conf, "Cannot open slurm log directory", INFO, 0);
        free(hostname);
        return (NULL);
    }
    while (!log_path) {
        if (!(entry = readdir(dir))) {
            write_log_to_file(demeter_conf, "Cannot find rotated slurm log file",  INFO, 0);
            free(hostname);
            closedir(dir);
            return (NULL);
        }
        if (strstr(entry->d_name, hostname) && strstr(entry->d_name, ".gz")) {
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
    if (!log_file)
        return (NULL);
    return (log_file);
}