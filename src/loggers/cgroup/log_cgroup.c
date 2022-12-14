// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such cgroup, much logging!
//___________________________________________________________________________________________________________________________________________

#include <string.h>
#include <src/slurmd/slurmd/slurmd.h>
#include "demeter.h"

// WRITES THE CGROUP DATA TO A TRANSFER FILE
//___________________________________________________________________________________________________________________________________________

static void write_cgroup_file(char *path, char *value)
{
    FILE *file;

    if (!path || !value)
        return;
    file = fopen(path, "a");
    if (!file)
        return;
    fprintf(file, "%s", value);
    fclose(file);
}

void transfer_log_cgroup(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf)
{
    char log_msg[1000], *file_path;

    memset(log_msg, 0, 1000);
    if (!cgroup_data || !job_info || !conf)
        return;
    if (job_info->step_id >= 4294967292)
        sprintf(log_msg, "\n###-1,%u,%d,%d,%d,#%s#,#%s####\n",
        cgroup_data->mem_max_usage_bytes, cgroup_data->oom_kill_disable, cgroup_data->under_oom, cgroup_data->oom_kill, cgroup_data->cpuset_cpus, cgroup_data->cpuset_effective_cpus);
    else
        sprintf(log_msg, "\n###%u,%u,%d,%d,%d,#%s#,#%s####\n",
        job_info->step_id, cgroup_data->mem_max_usage_bytes, cgroup_data->oom_kill_disable, cgroup_data->under_oom, cgroup_data->oom_kill, cgroup_data->cpuset_cpus, cgroup_data->cpuset_effective_cpus);
    file_path = get_job_transfer_path(job_info->job_id);
    write_cgroup_file(file_path, log_msg);
    free(file_path);
}