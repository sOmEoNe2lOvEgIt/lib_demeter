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
(demeter_conf_t *demeter_conf, job_id_info_t *job_info, linked_list_t *log_list)
{
    static char *log_buffer = NULL; // to get replaced by a fd whith popen command on log files.
    // Log files path to be specified in demeter.conf.
    static size_t buffer_len;
    parsed_log_t *curr_log = NULL;
    static int len = 0;

    log_buffer = read_sys_logs(); // to get replaced by a fd whith popen command on log files.
    if (log_buffer == NULL)
        return (NULL);
    buffer_len = strlen(log_buffer);
    log_list = add_to_list(log_list, init_parsed_log());
    for (int i = 0; log_buffer[i] != '\0'; i++) {
        curr_log = (parsed_log_t *)log_list->data;
        len = get_len_to_char(&log_buffer[i], '\n');
        if (len <= 2)
            continue;
        curr_log->unparsed_log = strndup(log_buffer + i, len);
        if (curr_log->unparsed_log == NULL)
            continue;
        if (is_log_empty(curr_log->unparsed_log)) {
            free(curr_log->unparsed_log);
            curr_log->unparsed_log = NULL;
            if (i + len < buffer_len)
                i += len - 1;
            continue;
        }
        curr_log->log_source_path = strdup("syslog");
        if (!handle_sys_log_level(curr_log, demeter_conf))
            continue;
        if (!handle_log_time(job_info, curr_log, demeter_conf))
            continue;
        curr_log->log_proc_name = strdup("kernel");
        log_list = add_to_list(log_list, init_parsed_log());
        if (i + len < buffer_len)
            i += len - 1;
    }
    free(log_buffer);
    return (log_list);
}