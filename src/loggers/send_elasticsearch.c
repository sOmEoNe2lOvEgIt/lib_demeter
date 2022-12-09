// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such send, much elastic!
//___________________________________________________________________________________________________________________________________________

#define _GNU_SOURCE
#include <stdio.h>
#include <curl/curl.h>
#include <src/common/macros.h>
#include "demeter.h"

// CALLS FORMATTERS AND SENDS DATA
//___________________________________________________________________________________________________________________________________________

int send_elastic(demeter_conf_t *demeter_conf ,job_id_info_t *job_info, linked_list_t *cgroup_data,
linked_list_t *gathered_slurm_logs, log_counter_t *slurm_log_counter,
linked_list_t *gathered_sys_logs, log_counter_t *sys_log_counter,
linked_list_t *gathered_sel, perf_data_t *gathered_perf_data)
{
    char *json_log, *tmp;

    json_log = strdup("{");
    tmp = format_job_info(job_info);
    json_log = append_str(json_log, tmp);
    free(tmp);
    json_log = append_str(json_log, strdup("\"job_data\":{"));
    tmp = format_cgroup(cgroup_data);
    json_log = append_str(json_log, tmp);
    if (tmp) {
        free(tmp);
        json_log = append_str(json_log, ", ");
    }
    tmp = format_logs(gathered_slurm_logs, false);
    json_log = append_str(json_log, tmp);
    if (tmp) {
        free(tmp);
        json_log = append_str(json_log, ", ");
    }
    tmp = format_log_counter(slurm_log_counter, false);
    json_log = append_str(json_log, tmp);
    if (tmp) {
        free(tmp);
        json_log = append_str(json_log, ", ");
    }
    tmp = format_logs(gathered_sys_logs, true);
    json_log = append_str(json_log, tmp);
    if (tmp) {
        free(tmp);
        json_log = append_str(json_log, ", ");
    }
    tmp = format_log_counter(sys_log_counter, true);
    json_log = append_str(json_log, tmp);
    if (tmp) {
        free(tmp);
        json_log = append_str(json_log, ", ");
    }
    tmp = format_sel_logs(gathered_sel);
    json_log = append_str(json_log, tmp);
    if (tmp) {
        free(tmp);
        json_log = append_str(json_log, ", ");
    }
    tmp = format_sel_count(gathered_perf_data);
    json_log = append_str(json_log, tmp);
    if (tmp)
        free(tmp);
    json_log = append_str(json_log, "}}");
    if (send_json(demeter_conf, json_log, job_info) == false)
        write_log_to_file(demeter_conf, "Failed to send log to elastic", DEBUG, 2);
    write_log_to_file(demeter_conf, json_log, DEBUG, 0);
    if (json_log)
        free(json_log);
    return (0);
}
