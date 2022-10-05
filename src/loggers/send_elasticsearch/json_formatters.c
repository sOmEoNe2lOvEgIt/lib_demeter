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

char *format_logs(linked_list_t *gathered_logs, linked_list_t *gathered_sel)
{
    linked_list_t *tmp = gathered_logs;
    char *json_log = NULL, element_str[50];

    if (tmp != NULL)
        json_log = strdup("\"syslogs/slurmlogs\":{\"data\":\"");
    for (;tmp != NULL; tmp = tmp->next) {
        if (((parsed_log_t *)tmp->data)->unparsed_log == NULL)
            continue;
        remove_newline(((parsed_log_t *)tmp->data)->unparsed_log);
        json_log = append_str(json_log, ((parsed_log_t *)tmp->data)->unparsed_log);
        json_log = append_str(json_log, "\\n");
    }
    if (gathered_logs != NULL)
        json_log = append_str(json_log, "\"}, ");
    tmp = gathered_sel;
    if (tmp != NULL) {
        sprintf(element_str, "\"sel_logs\":{\"data\":\"");
        json_log = append_str(json_log, element_str);
    }
    for (;tmp != NULL; tmp = tmp->next) {
        if (((parsed_sel_t *)tmp->data)->unparsed_sel == NULL)
            continue;
        remove_newline(((parsed_sel_t *)tmp->data)->unparsed_sel);
        json_log = append_str(json_log, ((parsed_sel_t *)tmp->data)->unparsed_sel);
        json_log = append_str(json_log, "\\n");
    }
    if (gathered_sel != NULL)
        json_log = append_str(json_log, "\"}");
    return (json_log);
}

static char *format_cgroup_single(cgroup_data_t *cgroup)
{
    char tmp[200];

    if (cgroup == NULL)
        return (NULL);
    memset(tmp, 0, 200);
    if (cgroup->step_id < 4294967200)
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

    if (tmp == NULL)
        return (NULL);
    json_cgroup = strdup("\"cgroup\":{");
    for (;tmp != NULL; tmp = tmp->next) {
        json_cgroup = append_str(json_cgroup, format_cgroup_single((cgroup_data_t *)tmp->data));
        if (tmp->next != NULL)
            json_cgroup = append_str(json_cgroup, ", ");
    }
    json_cgroup = append_str(json_cgroup, "}");
    return (json_cgroup);
}

char *format_job_info(job_id_info_t *job_info)
{
    char tmp[1224], hostname[1024];

    memset(tmp, 0, 200);
    gethostname(hostname, 1024);
    sprintf(tmp, "\"job_id\": \"%u\", \"user_id\":%u, \"hostname\":\"%s\", ", job_info->job_id, job_info->uid, hostname);
    return (strdup(tmp));
}

char *format_log_counter(log_counter_t *log_counter)
{
    char tmp[200];

    if (log_counter == NULL)
        return (NULL);
    memset(tmp, 0, 200);
    sprintf(tmp, "\"log_counter\":{\"errors\" : %u, \"warnings\" : %u, \"infos\" : %u, \"debugs\" : %u, \"fatals\" : %u}",
    log_counter->errors, log_counter->warnings, log_counter->infos, log_counter->debugs, log_counter->fatals);
    return (strdup(tmp));   
}

char *format_sys_log_counter(kern_log_counter_t *log_counter)
{
    char tmp[200];

    if (log_counter == NULL)
        return (NULL);
    memset(tmp, 0, 200);
    sprintf(tmp, "\"sys_log_counter\":{\"emergencies\" : %u, \"alerts\" : %u, \"criticals\" : %u, \"errors\" : %u, \"warnings\" : %u, \"notices\" : %u, \"infos\" : %u, \"debugs\" : %u}",
    log_counter->emerg, log_counter->alert, log_counter->crit, log_counter->err, log_counter->warning, log_counter->notice, log_counter->info, log_counter->debug);
    return (strdup(tmp));   
}

// to be completed
char *format_perf_count(perf_data_t *perf_data)
{
    char tmp[631];

    if (perf_data == NULL)
        return (strdup("\"perf_data\":{}"));
    memset(tmp, 0, 631);
    sprintf (tmp, "\"perf_count\":{\"counterselect\" : %u, \"excbufoverrunerrors\" : %u, \"linkdowned\" : %u, \"linkintegrityerrors\" : %u, \"linkrecovers\" : %u, \"portbufferoverrunerrors\" : %u, \"portdlidmappingerrors\" : %u, \"portlocalphysicalerrors\" : %u, \"portloopingerrors\" : %u, \"portmalformedpkterrors\" : %u, \"portselect\" : %u, \"portvlmappingerrors\" : %u, \"qp1dropped\" : %u, \"rcvconstrainterrors\" : %u, \"rcvdata\" : %u, \"rcverrors\" : %u, \"rcvpkts\" : %u, \"rcvremotephyerrors\" : %u, \"rcvswrelayerrors\" : %u}",
    perf_data->counterselect, perf_data->excbufoverrunerrors, perf_data->linkdowned, perf_data->linkintegrityerrors, perf_data->linkrecovers, perf_data->portbufferoverrunerrors, perf_data->portdlidmappingerrors, perf_data->portlocalphysicalerrors, perf_data->portloopingerrors, perf_data->portmalformedpkterrors, perf_data->portselect, perf_data->portvlmappingerrors, perf_data->qp1dropped, perf_data->rcvconstrainterrors, perf_data->rcvdata, perf_data->rcverrors, perf_data->rcvpkts, perf_data->rcvremotephyerrors, perf_data->rcvswrelayerrors);
    return (strdup(tmp));
}