// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, good slurm!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <syslog.h>
#include "slurm/slurm.h"
#include "demeter.h"

static bool handle_log_time(parsed_log_t *curr_log, demeter_conf_t *demeter_conf, time_t start_time)
{
    if (get_slurm_log_time(curr_log, start_time) != 0) {
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

static void caught_oom_kill(parsed_log_t *curr_log, cgroup_data_t *cgroup_data, demeter_conf_t *demeter_conf)

{
    int temp_log_ptr = 0;

    if (curr_log == NULL || cgroup_data == NULL || curr_log->unparsed_log == NULL)
        return;
    if (strstr(curr_log->unparsed_log, "oom-kill") != NULL) {
        cgroup_data->under_oom = 1;
        for (; curr_log->unparsed_log[temp_log_ptr] != 'd' && curr_log->unparsed_log[temp_log_ptr] != '\0'; temp_log_ptr++);
        temp_log_ptr += 2;
        cgroup_data->oom_kill = atoi(&curr_log->unparsed_log[temp_log_ptr]);
    }
}

linked_list_t *gather_slurm_logs
(demeter_conf_t *demeter_conf, job_id_info_t *job_info, cgroup_data_t *cgroup_data, linked_list_t *log_list)
{
    FILE *log_file = NULL;
    parsed_log_t *curr_log = NULL;
    char *buffer = NULL;
    size_t len = 1000;

    if ((log_file = fopen(demeter_conf->slurm_log_path, "r")) == NULL) {
        write_log_to_file(demeter_conf, "Cannot open slurm log file", DEBUG, 3);
        write_log_to_file(demeter_conf, demeter_conf->slurm_log_path, DEBUG, 99);
        return (NULL);
    }
    log_list = add_to_list(log_list, init_parsed_log());
    while (getline(&buffer, &len, log_file) != -1) {
        curr_log = (parsed_log_t *)log_list->data;
        curr_log->unparsed_log = strdup(buffer);
        curr_log->log_source_path = strdup("slurm_log_path");
        if (!handle_log_level(curr_log, demeter_conf))
            continue;
        if (!handle_log_time(curr_log, demeter_conf, job_info->start_time))
            continue;
        curr_log->log_proc_name = strdup("slurm");
        caught_oom_kill(curr_log, cgroup_data, demeter_conf);
        log_list = add_to_list(log_list, init_parsed_log());
    }
    fclose(log_file);
    return (log_list);
}