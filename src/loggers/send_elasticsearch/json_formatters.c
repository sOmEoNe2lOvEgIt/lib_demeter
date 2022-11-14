// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such json, good format!
//___________________________________________________________________________________________________________________________________________

#define _GNU_SOURCE
#include <stdio.h>
#include <curl/curl.h>
#include "demeter.h"
#include "src/common/macros.h"

// FORMATS LOGS TO JSON FORMAT FOR ELASTICSEARCH
//___________________________________________________________________________________________________________________________________________

char *format_logs(linked_list_t *gathered_logs, bool is_syslog)
{
    linked_list_t *tmp = gathered_logs;
    char *json_log = NULL;

    if (tmp) {
        if (is_syslog)
            json_log = strdup("\"syslogs\":{\"data\":\"");
        else
            json_log = strdup("\"slurmlogs\":{\"data\":\"");
    }
    for (;tmp; tmp = tmp->next) {
        if (((parsed_log_t *)tmp->data)->unparsed_log == NULL)
            continue;
        remove_newline(((parsed_log_t *)tmp->data)->unparsed_log);
        json_log = append_str(json_log, ((parsed_log_t *)tmp->data)->unparsed_log);
        json_log = append_str(json_log, "\\n");
    }
    if (gathered_logs)
        json_log = append_str(json_log, "\"}");
    return (json_log);
}

// FORMATS SEL LOGS TO JSON FORMAT FOR ELASTICSEARCH
//___________________________________________________________________________________________________________________________________________

char *format_sel_logs(linked_list_t *gathered_sel)
{
    linked_list_t *tmp = gathered_sel;
    char *json_log = NULL;

    if (tmp)
        json_log = strdup("\"sel_logs\":{\"data\":\"");
    for (;tmp; tmp = tmp->next) {
        if (((parsed_sel_t *)tmp->data)->unparsed_sel == NULL)
            continue;
        remove_newline(((parsed_sel_t *)tmp->data)->unparsed_sel);
        json_log = append_str(json_log, ((parsed_sel_t *)tmp->data)->unparsed_sel);
        json_log = append_str(json_log, "\\n");
    }
    if (gathered_sel)
        json_log = append_str(json_log, "\"}");
    return (json_log);
}

// FORMATS CGROUP DATA TO JSON FORMAT FOR ELASTICSEARCH
//___________________________________________________________________________________________________________________________________________

static char *format_cgroup_single(cgroup_data_t *cgroup)
{
    char tmp[200];

    if (!cgroup)
        return (NULL);
    memset(tmp, 0, 200);
    if (cgroup->step_id < (UINT_MAX - 1))
        sprintf(tmp, "\"step_%u\":{\"cpuset_cpus\":\"%s\", \"cpuset_effective_cpus\":\"%s\", \"mem_max_usage_bytes\":%u, \"oom_kill\":%u, \"oom_kill_disable\":%u, \"under_oom\":%u}",
        cgroup->step_id, cgroup->cpuset_cpus, cgroup->cpuset_effective_cpus, cgroup->mem_max_usage_bytes, cgroup->oom_kill, cgroup->oom_kill_disable, cgroup->under_oom);
    else
        sprintf(tmp, "\"no_step\":{\"cpuset_cpus\":\"%s\", \"cpuset_effective_cpus\":\"%s\", \"mem_max_usage_bytes\":%u, \"oom_kill\":%u, \"oom_kill_disable\":%u, \"under_oom\":%u}",
        cgroup->cpuset_cpus, cgroup->cpuset_effective_cpus, cgroup->mem_max_usage_bytes, cgroup->oom_kill, cgroup->oom_kill_disable, cgroup->under_oom);
    return (strdup(tmp));
}

char *format_cgroup(linked_list_t *cgroup)
{
    linked_list_t *tmp = cgroup;
    char *json_cgroup = NULL;

    if (!tmp)
        return (strdup("\"cgroup\":{}"));
    json_cgroup = strdup("\"cgroup\":{");
    for (;tmp; tmp = tmp->next) {
        json_cgroup = append_str(json_cgroup, format_cgroup_single((cgroup_data_t *)tmp->data));
        if (tmp->next && json_cgroup[strlen(json_cgroup) - 1] == '}')
            json_cgroup = append_str(json_cgroup, ", ");
    }
    json_cgroup = append_str(json_cgroup, "}");
    return (json_cgroup);
}

// FORMATS JOB DATA TO JSON FORMAT FOR ELASTICSEARCH
//___________________________________________________________________________________________________________________________________________

char *format_job_info(job_id_info_t *job_info)
{
    char tmp[454], hostname[254];

    memset(tmp, 0, 454);
    gethostname(hostname, 254);
    sprintf(tmp, "\"job_id\": \"%u\", \"user_id\":%u, \"hostname\":\"%s\", ", job_info->job_id, job_info->uid, hostname);
    return (strdup(tmp));
}

// FORMATS LOG COUNTERS TO JSON FORMAT FOR ELASTICSEARCH
//___________________________________________________________________________________________________________________________________________

char *format_log_counter(log_counter_t *log_counter, bool is_syslog)
{
    char tmp[200];

    if (!log_counter)
        return (strdup("\"no_log_counter\":{}"));
    memset(tmp, 0, 200);
    if (is_syslog)
        sprintf(tmp, "\"sys_log_counter\":{\"errors\" : %u, \"warnings\" : %u, \"infos\" : %u, \"debugs\" : %u, \"fatals\" : %u}",
        log_counter->errors, log_counter->warnings, log_counter->infos, log_counter->debugs, log_counter->fatals);
    else
        sprintf(tmp, "\"slurm_log_counter\":{\"errors\" : %u, \"warnings\" : %u, \"infos\" : %u, \"debugs\" : %u, \"fatals\" : %u}",
        log_counter->errors, log_counter->warnings, log_counter->infos, log_counter->debugs, log_counter->fatals);
    return (strdup(tmp));   
}

// FORMATS PERFORMANCE DATA TO JSON FORMAT FOR ELASTICSEARCH
//___________________________________________________________________________________________________________________________________________
// to be completed
char *format_sel_count(perf_data_t *perf_data)
{
    char tmp[631];

    if (!perf_data)
        return (strdup("\"sel_count\":{}"));
    memset(tmp, 0, 631);
    sprintf (tmp, "\"sel_count\":{\"counterselect\" : %u, \"excbufoverrunerrors\" : %u, \"linkdowned\" : %u, \"linkintegrityerrors\" : %u, \"linkrecovers\" : %u, \"portbufferoverrunerrors\" : %u, \"portdlidmappingerrors\" : %u, \"portlocalphysicalerrors\" : %u, \"portloopingerrors\" : %u, \"portmalformedpkterrors\" : %u, \"portselect\" : %u, \"portvlmappingerrors\" : %u, \"qp1dropped\" : %u, \"rcvconstrainterrors\" : %u, \"rcvdata\" : %u, \"rcverrors\" : %u, \"rcvpkts\" : %u, \"rcvremotephyerrors\" : %u, \"rcvswrelayerrors\" : %u}",
    perf_data->counterselect, perf_data->excbufoverrunerrors, perf_data->linkdowned, perf_data->linkintegrityerrors, perf_data->linkrecovers, perf_data->portbufferoverrunerrors, perf_data->portdlidmappingerrors, perf_data->portlocalphysicalerrors, perf_data->portloopingerrors, perf_data->portmalformedpkterrors, perf_data->portselect, perf_data->portvlmappingerrors, perf_data->qp1dropped, perf_data->rcvconstrainterrors, perf_data->rcvdata, perf_data->rcverrors, perf_data->rcvpkts, perf_data->rcvremotephyerrors, perf_data->rcvswrelayerrors);
    return (strdup(tmp));
}