// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, nice kernel!
//___________________________________________________________________________________________________________________________________________

#define _GNU_SOURCE
#include <stdio.h>
#include <syslog.h>
#include <zlib.h>
#include "slurm/slurm.h"
#include "demeter.h"

static bool handle_log_time(job_id_info_t *job_info, parsed_log_t *curr_log, demeter_conf_t *demeter_conf)
{
    if (get_sys_log_time(curr_log, job_info->start_time) != 0) {
        write_log_to_file(demeter_conf, "Cannot get log time/log is not written at runtime", DEBUG, 99);
        if (curr_log->unparsed_log != NULL) {
            free(curr_log->unparsed_log);
            curr_log->unparsed_log = NULL;
        }
        if (curr_log->log_source_path != NULL) {
            free(curr_log->log_source_path);
            curr_log->log_source_path = NULL;
        }
        return false;
    }
    return true;
}

// linked_list_t *gather_kernel_logs
// (demeter_conf_t *demeter_conf, job_id_info_t *job_info, linked_list_t *log_list)
// {
//     static char *log_buffer = NULL;
//     static size_t buffer_len;
//     parsed_log_t *curr_log = NULL;
//     static int len = 0;

//     log_buffer = read_sys_logs();
//     if (log_buffer == NULL)
//         return (NULL);
//     buffer_len = strlen(log_buffer);
//     log_list = add_to_list(log_list, init_parsed_log());
//     for (int i = 0; log_buffer[i] != '\0'; i++) {
//         curr_log = (parsed_log_t *)log_list->data;
//         len = get_len_to_char(&log_buffer[i], '\n');
//         if (len <= 2)
//             continue;
//         curr_log->unparsed_log = strndup(log_buffer + i, len);
//         if (curr_log->unparsed_log == NULL)
//             continue;
//         if (is_log_empty(curr_log->unparsed_log)) {
//             free(curr_log->unparsed_log);
//             curr_log->unparsed_log = NULL;
//             if (i + len < buffer_len)
//                 i += len - 1;
//             continue;
//         }
//         curr_log->log_source_path = strdup("syslog");
//         if (!handle_sys_log_level(curr_log, demeter_conf))
//             continue;
//         if (!handle_log_time(job_info, curr_log, demeter_conf))
//             continue;
//         curr_log->log_proc_name = strdup("kernel");
//         log_list = add_to_list(log_list, init_parsed_log());
//         if (i + len < buffer_len)
//             i += len - 1;
//     }
//     free(log_buffer);
//     return (log_list);
// }

char *get_basic_syslog_path(demeter_conf_t *demeter_conf)
{
    char *sys_logs_path = NULL;

    if (demeter_conf->sys_log_path == NULL)
        return (NULL);
    asprintf(&sys_logs_path, "%s%s", demeter_conf->sys_log_path, "syslog");
    if (sys_logs_path == NULL)
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
    if (log_path == NULL)
        return (NULL);
    if ((dir = opendir(demeter_conf->sys_log_path)) == NULL) {
        write_log_to_file(demeter_conf, "Cannot open system log directory", WARNING, 0);
        free(log_path);
        return (NULL);
    }
    while (!found_rotated_log) {
        if ((entry = readdir(dir)) == NULL) {
            write_log_to_file(demeter_conf, "Cannot find system log file", WARNING, 0);
            free(log_path);
            closedir(dir);
            return (NULL);
        }
        if (strstr(entry->d_name, "syslog") != NULL && strstr(entry->d_name, ".gz") != NULL) {
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
    if (log_file == NULL)
        return (NULL);
    return (log_file);
}

static FILE *open_system_logs(demeter_conf_t *demeter_conf)
{
    FILE *sys_logs = NULL;
    char *sys_logs_path = NULL;

    if ((sys_logs_path = get_basic_syslog_path(demeter_conf)) == NULL)
        return (NULL);
    sys_logs = fopen(sys_logs_path, "r");
    if (sys_logs == NULL)
        return (NULL);
    free(sys_logs_path);
    return (sys_logs);
}

static bool handle_log(long ln_offset, char *buffer, linked_list_t **log_list,
demeter_conf_t *demeter_conf, job_id_info_t *job_info)
{
    parsed_log_t *curr_log = NULL;

    curr_log = (parsed_log_t *)(*log_list)->data;
    curr_log->unparsed_log = strdup(buffer);
    if (curr_log->unparsed_log == NULL)
        return (false);
    if (is_log_empty(curr_log->unparsed_log)) {
        free(curr_log->unparsed_log);
        curr_log->unparsed_log = NULL;
        return (true);
    }
    curr_log->log_source_path = strdup("syslog-path");
    if (!handle_log_level(curr_log, demeter_conf))
        return (true);
    if (handle_log_time(job_info, curr_log, demeter_conf))
        return (false);
    curr_log->log_proc_name = strdup("system");
    (*log_list) = add_to_list((*log_list), init_parsed_log());
    return (true);
}

linked_list_t *gather_system_logs
(demeter_conf_t *demeter_conf, job_id_info_t *job_info, linked_list_t *log_list)
{
    FILE *log_file = NULL;
    gzFile gz_log_file = NULL;
    long ln_offset = 0;
    char *buffer = NULL;
    size_t len = 1000;
    int zgetline_ret = 0;

    if ((log_file = open_system_logs(demeter_conf)) == NULL)
        return (NULL);
    log_list = add_to_list(log_list, init_parsed_log());
    for (ln_offset = 0; getline_from_end(&buffer, &len, log_file, &ln_offset) != -1; ln_offset++){
        if (!handle_log(ln_offset, buffer, &log_list, demeter_conf, job_info)) {
            fclose(log_file);
            free(buffer);
            return (log_list);
        }
    }
    fclose(log_file);
    if (buffer != NULL) {
        free(buffer);
        buffer = NULL;
    }
    if ((gz_log_file = open_rotated_slurm_log(demeter_conf, job_info)) == NULL)
        return (log_list);
    write_log_to_file(demeter_conf, "Reading rotated system log file", INFO, 0);
    for (ln_offset = 0; (zgetline_ret = zgetline_from_end(&buffer, &len, gz_log_file, &ln_offset)) != -1; ln_offset++){
        write_log_to_file(demeter_conf, "Got compressed line", INFO, 0);
        if (!handle_log(ln_offset, buffer, &log_list, demeter_conf, job_info)) {
            gzclose(gz_log_file);
            free(buffer);
            return (log_list);
        }
        if (zgetline_ret == -42)
            break;
    }
    gzclose(gz_log_file);
    if (buffer != NULL)
        free(buffer);
    return (log_list);
}