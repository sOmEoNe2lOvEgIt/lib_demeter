// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, nice kernel!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <syslog.h>
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

linked_list_t *gather_kernel_logs
(demeter_conf_t *demeter_conf, job_id_info_t *job_info,
cgroup_data_t *cgroup_data, linked_list_t *log_list)
{
    static char *log_buffer = NULL;
    parsed_log_t *curr_log = NULL;
    static int len = 0;

    if ((log_buffer = read_sys_logs()) == NULL)
        return (NULL);  
    log_list = add_to_list(log_list, init_parsed_log());
    for (int i = 0; log_buffer[i] != '\0'; i++) {
        curr_log = (parsed_log_t *)log_list->data;
        for (len = 0; log_buffer[i + len] != '\0' && log_buffer[i + len] != '\n'; len++);
        if (len <= 2)
            continue;
        curr_log->unparsed_log = strndup(log_buffer + i, len);
        i += len - 1;
        if (curr_log->unparsed_log == NULL || is_log_empty(curr_log->unparsed_log))
            continue;
        curr_log->log_source_path = strdup("syslog");
        if (!handle_log_level(curr_log, demeter_conf))
            continue;
        if (!handle_log_time(job_info, curr_log, demeter_conf))
            continue;
        curr_log->log_proc_name = strdup("kernel");
        log_list = add_to_list(log_list, init_parsed_log());
    }
    free(log_buffer);
    return (log_list);
}