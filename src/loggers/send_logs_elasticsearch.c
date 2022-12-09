// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such send, much elastic!
//___________________________________________________________________________________________________________________________________________

#define _GNU_SOURCE
#include <stdio.h>
#include <curl/curl.h>
#include <src/common/macros.h>
#include <src/common/log.h>
#include "demeter.h"

// CALLS FORMATTERS AND SENDS LOGS
//___________________________________________________________________________________________________________________________________________

int send_logs_elastic(demeter_conf_t *demeter_conf ,job_id_info_t *job_info, linked_list_t *gathered_slurm_logs,
linked_list_t *gathered_sys_logs, linked_list_t *gathered_sel)
{
    char *json_log = NULL, tmp[300], hostname[254];

    if (gethostname(hostname, 254) == -1)
        return (1);
    info ("demeter: Sending logs to elasticsearch");
    for (; gathered_slurm_logs; gathered_slurm_logs = gathered_slurm_logs->next) {
        if (!gathered_slurm_logs || !gathered_slurm_logs->data)
            continue;
        if (!((parsed_log_t *)gathered_slurm_logs->data)->data)
            continue;
        memset(tmp, 0, 100);
        json_log = strdup("{");
        sprintf(tmp, "\"job_id\":%d, \"hostname\":\"%s\", ", job_info->job_id, hostname);
        json_log = append_str(json_log, tmp);
        json_log = append_str(json_log, "\"log_type\":\"slurm\", ");
        json_log = append_str(json_log, "\"log_time\":\"");
        json_log = append_str(json_log, ((parsed_log_t *)gathered_slurm_logs->data)->log_time_str);
        if (((parsed_log_t *)gathered_slurm_logs->data)->error_code) {
            json_log = append_str(json_log, "\", \"error_code\":");
            sprintf(tmp, "%d", ((parsed_log_t *)gathered_slurm_logs->data)->error_code);
            json_log = append_str(json_log, tmp);
            json_log  = append_str(json_log, ",");
        } else
            json_log = append_str(json_log, "\",");
        json_log = append_str(json_log, " \"log_data\":\"");
        json_log = append_str(json_log, ((parsed_log_t *)gathered_slurm_logs->data)->data);
        json_log = append_str(json_log, "\"}");
        if (send_json(demeter_conf, json_log, job_info))
            return (1);
        if (json_log)
            free(json_log);
        json_log = NULL;
    }
    for (; gathered_sys_logs; gathered_sys_logs = gathered_sys_logs->next) {
        if (!gathered_sys_logs || !gathered_sys_logs->data)
            continue;
        if (!((parsed_log_t *)gathered_sys_logs->data)->data)
            continue;
        memset(tmp, 0, 100);
        json_log = strdup("{");
        sprintf(tmp, "\"job_id\":%d, \"hostname\":\"%s\", ", job_info->job_id, hostname);
        json_log = append_str(json_log, tmp);
        json_log = append_str(json_log, "\"log_type\":\"system\", ");
        json_log = append_str(json_log, "\"log_time\":\"");
        if (((parsed_log_t *)gathered_sys_logs->data)->error_code) {
            json_log = append_str(json_log, "\", \"error_code\":");
            sprintf(tmp, "%d", ((parsed_log_t *)gathered_sys_logs->data)->error_code);
            json_log = append_str(json_log, tmp);
            json_log  = append_str(json_log, ",");
        } else
            json_log = append_str(json_log, "\",");
        json_log = append_str(json_log, " \"log_data\":\"");
        json_log = append_str(json_log, ((parsed_log_t *)gathered_sys_logs->data)->data);
        json_log = append_str(json_log, "\"}");
        if (send_json(demeter_conf, json_log, job_info))
            return (1);
        if (json_log)
            free(json_log);
        json_log = NULL;
    }
    return (0);
}