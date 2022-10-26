// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such cgroup, much unlogging!
//___________________________________________________________________________________________________________________________________________

#include <string.h>
#include <stdio.h>
#include "demeter.h"

// UNLOGS CGROUP DATA FROM A FILE
//___________________________________________________________________________________________________________________________________________

linked_list_t *unlog_cgroup(demeter_conf_t *demeter_conf, uint job_id)
{
    FILE *file = NULL;
    char *line = NULL, *cgroup_path = NULL,
    cpuset_cpus[1024] = {0}, cpuset_effective_cpus[1024] = {0};
    size_t len = 1000;
    signed long long int step_id = 0;
    linked_list_t *list = NULL;
    cgroup_data_t *cgroup_data = NULL;

    if (!demeter_conf)
        return NULL;
    cgroup_path = get_job_transfer_path(job_id);
    if (!cgroup_path)
        return NULL;
    file = fopen(cgroup_path, "r");
    if (!file)
        return NULL;
    while (getline(&line, &len, file) != -1) {
        if (strstr(line, "###")) {
            cgroup_data = init_cgroup_struct();
            if (!cgroup_data) {
                fclose(file);
                free(line);
                return NULL;
            }
            line += get_len_to_char(line, '#');
            sscanf(line, "###%lli,%u,%d,%d,%d,#%[^#]#,#%[^#]####",
            &step_id, &cgroup_data->mem_max_usage_bytes, &cgroup_data->oom_kill_disable,
            &cgroup_data->under_oom, &cgroup_data->oom_kill,
            cpuset_cpus, cpuset_effective_cpus);
            cgroup_data->cpuset_cpus = strdup(cpuset_cpus);
            cgroup_data->cpuset_effective_cpus = strdup(cpuset_effective_cpus);
            if (step_id < 0)
                cgroup_data->step_id = UINT_MAX -1;
            else
                cgroup_data->step_id = step_id;
            list = add_to_list(list, cgroup_data);
        }
    }
    fclose(file);
    remove(cgroup_path);
    free (cgroup_path);
    if (line)
        free (line);
    return (list);
}