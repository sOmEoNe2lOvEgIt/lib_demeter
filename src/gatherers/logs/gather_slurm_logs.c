// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, good slurm!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <syslog.h>
#include "slurm/slurm.h"
#include "demeter.h"

static int handle_log_time(parsed_log_t *curr_log, demeter_conf_t *demeter_conf, time_t start_time)
{
    int ret = 0;

    ret = get_slurm_log_time(curr_log, start_time);
    if (ret) {
        write_log_to_file(demeter_conf, "Cannot get log time", DEBUG, 99);
        if (curr_log->unparsed_log != NULL) {
            free(curr_log->unparsed_log);
            curr_log->unparsed_log = NULL;
        }
        if (curr_log->log_source_path != NULL) {
            free(curr_log->log_source_path);
            curr_log->log_source_path = NULL;
        }
        return (ret);
    }
    return (0);
}

linked_list_t *gather_slurm_logs
(demeter_conf_t *demeter_conf, job_id_info_t *job_info, linked_list_t *log_list)
{
    FILE *log_file = NULL;
    parsed_log_t *curr_log = NULL;
    int log_time_code = 0;
    char *buffer = NULL;
    long last_line = 0;
    size_t len = 1000;

    if ((log_file = fopen(demeter_conf->slurm_log_path, "r")) == NULL) { // gonna get replaced with popen to read log from the end
                                                                         // and handle compressed log files.
        write_log_to_file(demeter_conf, "Cannot open slurm log file", INFO, 0);
        write_log_to_file(demeter_conf, demeter_conf->slurm_log_path, DEBUG, 99);
        return (NULL);
    }
    log_list = add_to_list(log_list, init_parsed_log());
    while (getline_from_end(&buffer, &len, log_file, &last_line) != -1) {
        last_line++;
        curr_log = (parsed_log_t *)log_list->data;
        curr_log->unparsed_log = strdup(buffer);
        if (curr_log->unparsed_log == NULL)
            continue;
        if (is_log_empty(curr_log->unparsed_log)) {
            free(curr_log->unparsed_log);
            curr_log->unparsed_log = NULL;
            continue;
        }
        curr_log->unparsed_log[strlen(buffer)] = '\0';
        curr_log->log_source_path = strdup("slurm_log_path");
        if (!handle_log_level(curr_log, demeter_conf))
            continue;
        log_time_code = handle_log_time(curr_log, demeter_conf, job_info->start_time);
        if (log_time_code == 1) {
            continue;
        }
        if (log_time_code == 2) {
            break;
        }
        curr_log->log_proc_name = strdup("slurm");
        log_list = add_to_list(log_list, init_parsed_log());
    }
    fclose(log_file);
    if (buffer != NULL)
        free(buffer);
    return (log_list);
}