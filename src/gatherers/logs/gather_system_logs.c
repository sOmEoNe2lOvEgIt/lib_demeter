// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, nice system!
//___________________________________________________________________________________________________________________________________________

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