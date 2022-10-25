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
    if (conf->log_file_path != NULL)
        free(conf->log_file_path);
    if (conf->slurm_log_path != NULL)
        free(conf->slurm_log_path);
    if (conf->sys_log_path != NULL)
        free(conf->sys_log_path);
    if (conf->demeter_comp_loc != NULL)
        free(conf->demeter_comp_loc);
    if (conf->demeter_comp_proxy != NULL)
        free(conf->demeter_comp_proxy);
    free(conf);
}

void free_cgroup(cgroup_data_t *data)
{
    if (data == NULL)
        return;
    if (data->cpuset_cpus != NULL)
        free(data->cpuset_cpus);
    if (data->cpuset_effective_cpus != NULL)
        free(data->cpuset_effective_cpus);
    if (data != NULL)
        free(data);
}

void free_cgroup_list(linked_list_t *cgroup_list)
{
    linked_list_t *next_list = NULL;

    if (!cgroup_list|| !cgroup_list->data)
        return;
    for (;cgroup_list; cgroup_list = next_list) {
        next_list = cgroup_list->next;
        free_cgroup((cgroup_data_t *)cgroup_list->data);
        free(cgroup_list);
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
    if (log->unparsed_log != NULL)
        free(log->unparsed_log);
    if (log->log_proc_name != NULL)
        free(log->log_proc_name);
    if (log->log_source_path != NULL)
        free(log->log_source_path);
    if (log->log_time_str != NULL)
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
    if (parsed_sel->unparsed_sel != NULL)
        free(parsed_sel->unparsed_sel);
    if (parsed_sel->sel_time_str != NULL)
        free(parsed_sel->sel_time_str);
    if (parsed_sel->sel_msg != NULL)
        free(parsed_sel->sel_msg);
    if (parsed_sel->sel_msg_type != NULL)
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