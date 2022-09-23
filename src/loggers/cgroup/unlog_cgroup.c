// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such cgroup, much unlogging!
//___________________________________________________________________________________________________________________________________________

#include <string.h>
#include <stdio.h>
#include "demeter.h"

linked_list_t *unlog_cgroup(demeter_conf_t *demeter_conf)
{
    FILE *file = NULL;
    char *line = NULL;
    size_t len = 1000;
    signed long long int step_id = 0;
    linked_list_t *list = NULL;
    cgroup_data_t *cgroup_data = NULL;

    if (demeter_conf == NULL)
        return NULL;
    file = fopen(demeter_conf->log_file_path, "r");
    if (file == NULL)
        return NULL;
    while (getline(&line, &len, file) != -1) {
        if (strstr(line, "###") != NULL) {
            cgroup_data = alloc_cgroup_struct();
            if (cgroup_data == NULL)
                return NULL;
            line += get_len_to_char(line, '#');
            sscanf(line, "###%lli,%u,%d,%d,%d,#%[^#]#,#%[^#]#,###",
            &step_id, &cgroup_data->mem_max_usage_bytes, &cgroup_data->oom_kill_disable, &cgroup_data->under_oom, &cgroup_data->oom_kill, cgroup_data->cpuset_cpus, cgroup_data->cpuset_effective_cpus);
            if (step_id < 0)
                cgroup_data->step_id = 4294967200;
            else
                cgroup_data->step_id = step_id;
            list = add_to_list(list, cgroup_data);
        }
    }
    fclose(file);
    if (line != NULL)
        free (line);
    return (list);
}