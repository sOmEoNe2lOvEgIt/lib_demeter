// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such tools, nice system!
//___________________________________________________________________________________________________________________________________________

#define _GNU_SOURCE
#include <stdio.h>
#include <syslog.h>
#include <zlib.h>
#include "slurm/slurm.h"
#include "demeter.h"

static char *get_basic_syslog_path(demeter_conf_t *demeter_conf)
{
    char *sys_logs_path = NULL;

    if (!demeter_conf->sys_log_path)
        return (NULL);
    asprintf(&sys_logs_path, "%s%s", demeter_conf->sys_log_path, "syslog");
    if (!sys_logs_path)
        return (NULL);
    return (sys_logs_path);
}

gzFile open_rotated_system_log(demeter_conf_t *demeter_conf, job_id_info_t *job_info)
{
    char *log_path = NULL;
    struct dirent *entry = NULL;
    gzFile log_file = NULL;
    DIR *dir = NULL;
    time_t rotate_time = 0;
    bool found_rotated_log = false;

    log_path = get_basic_syslog_path(demeter_conf);
    if (!log_path)
        return (NULL);
    if (!(dir = opendir(demeter_conf->sys_log_path))) {
        write_log_to_file(demeter_conf, "Cannot open system log directory", WARNING, 0);
        free(log_path);
        return (NULL);
    }
    while (!found_rotated_log) {
        if (!(entry = readdir(dir))) {
            write_log_to_file(demeter_conf, "Cannot find system log file", WARNING, 0);
            free(log_path);
            closedir(dir);
            return (NULL);
        }
        if (strstr(entry->d_name, "syslog") && strstr(entry->d_name, ".gz")) {
            rotate_time = get_rotate_time(entry->d_name);
            if (rotate_time == -1) {
                write_log_to_file(demeter_conf, "Cannot get rotated system log file time", INFO, 0);
                free(log_path);
                closedir(dir);
                return (NULL);
            }
            if (rotate_time > job_info->start_time) {
                asprintf(&log_path, "%s%s", demeter_conf->sys_log_path, entry->d_name);
                found_rotated_log = true;
            }
            else 
                write_log_to_file(demeter_conf, "wrong time for file", INFO, 0);
        }
    }
    closedir(dir);
    log_file = gzopen(log_path, "r");
    free(log_path);
    if (!log_file)
        return (NULL);
    return (log_file);
}

FILE *open_system_logs(demeter_conf_t *demeter_conf)
{
    FILE *sys_logs = NULL;
    char *sys_logs_path = NULL;

    if (!(sys_logs_path = get_basic_syslog_path(demeter_conf)))
        return (NULL);
    sys_logs = fopen(sys_logs_path, "r");
    if (!sys_logs)
        return (NULL);
    free(sys_logs_path);
    return (sys_logs);
}