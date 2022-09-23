// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, much var, such free!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include "src/common/slurm_xlator.h"
#include "demeter.h"
#include "gather_ib.h"

void free_list(linked_list_t *list)
{
    linked_list_t *tmp = list, *prev = NULL;

    while (tmp != NULL) {
        prev = tmp;
        tmp = tmp->next;
        if (prev->data != NULL)
            free(prev->data);
        free(prev);
    }
}

void free_conf(demeter_conf_t *conf)
{
    if (conf == NULL)
        return;
    if (conf->slurm_log_path != NULL)
        free(conf->slurm_log_path);
    if (conf->log_file_path != NULL)
        free(conf->log_file_path);
    free(conf);
}

void free_cgroup(cgroup_data_t *data)
{
    if (data == NULL)
        return;
    if (!is_log_empty(data->cpuset_cpus))
        free(data->cpuset_cpus);
    if (!is_log_empty(data->cpuset_effective_cpus))
        free(data->cpuset_effective_cpus);
    if (data != NULL)
        free(data);
}

void free_cgroup_list(linked_list_t *cgroup_list)
{
    cgroup_data_t *curr_cgroup = NULL;
    linked_list_t *next_list = NULL;

    if (cgroup_list == NULL || cgroup_list->data == NULL)
        return;
    while (cgroup_list != NULL) {
        next_list = cgroup_list->next;
        if (cgroup_list->data != NULL) {
            curr_cgroup = (cgroup_data_t *)cgroup_list->data;
            free_cgroup(curr_cgroup);
        }
        free(cgroup_list);
        cgroup_list = next_list;
    }
}

void free_job_id_info(job_id_info_t *job_info)
{
    if (job_info == NULL)
        return;
    free(job_info);
}

// LOG
//___________________________________________________________________________________________________________________________________________

void free_parsed_log(parsed_log_t *log)
{
    if (log == NULL)
        return;
    if (!is_log_empty(log->unparsed_log))
        free(log->unparsed_log);
    if (!is_log_empty(log->log_proc_name))
        free(log->log_proc_name);
    if (!is_log_empty(log->log_source_path))
        free(log->log_source_path);
    if (!is_log_empty(log->log_time_str))
        free(log->log_time_str);
    if (log != NULL)
        free(log);
}

void free_log_list(linked_list_t *log_list)
{
    parsed_log_t *curr_log = NULL;
    linked_list_t *next_list = NULL;

    if (log_list == NULL || log_list->data == NULL)
        return;
    while (log_list != NULL) {
        next_list = log_list->next;
        if (log_list->data != NULL) {
            curr_log = (parsed_log_t *)log_list->data;
            free_parsed_log(curr_log);
        }
        free(log_list);
        log_list = next_list;
    }
}

void free_log_counter(log_counter_t *log_counter)
{
    if (log_counter == NULL)
        return;
    free(log_counter);
}

// SEL
//___________________________________________________________________________________________________________________________________________

void free_parsed_sel(parsed_sel_t *parsed_sel)
{
    if (parsed_sel == NULL)
        return;
    if (!is_log_empty(parsed_sel->unparsed_sel))
        free(parsed_sel->unparsed_sel);
    if (!is_log_empty(parsed_sel->sel_time_str))
        free(parsed_sel->sel_time_str);
    if (!is_log_empty(parsed_sel->sel_msg))
        free(parsed_sel->sel_msg);
    if (!is_log_empty(parsed_sel->sel_msg_type))
        free(parsed_sel->sel_msg_type);
    free(parsed_sel);
}

void free_sel_list(linked_list_t *sel_list)
{
    parsed_sel_t *curr_sel = NULL;
    linked_list_t *next_list = NULL;

    if (sel_list == NULL || sel_list->data == NULL)
        return;
    while (sel_list != NULL) {
        next_list = sel_list->next;
        if (sel_list->data != NULL) {
            curr_sel = (parsed_sel_t *)sel_list->data;
            free_parsed_sel(curr_sel);
        }
        free(sel_list);
        sel_list = next_list;
    }
}

// IB
//___________________________________________________________________________________________________________________________________________

void free_perf_count(perf_data_t *perf_count)
{
    if (perf_count != NULL)
        free(perf_count);
}