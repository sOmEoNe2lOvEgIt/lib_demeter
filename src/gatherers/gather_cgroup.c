// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such cgroup, much gathering!
//___________________________________________________________________________________________________________________________________________

#include <string.h>
#include "src/slurmd/slurmd/slurmd.h"
#include "demeter.h"

static cgroup_data_t *alloc_cgroup_struct(void)
{
    cgroup_data_t *cgroup_data = malloc(sizeof(cgroup_data_t));

    if (cgroup_data == NULL) {
        my_slurm_error("malloc failed");
        return (NULL);
    }
    cgroup_data->mem_max_usage_bytes = 0;
    cgroup_data->oom_kill_disable = 0;
    cgroup_data->under_oom = 0;
    cgroup_data->oom_kill = 0;
    cgroup_data->cpuset_cpus = NULL;
    cgroup_data->cpuset_effective_cpus = NULL;
    return (cgroup_data);
}

cgroup_data_t *gather_cgroup(job_id_info_t *job_info, demeter_conf_t *conf)
{
    cgroup_data_t *cgroup_data = NULL;

    cgroup_data = alloc_cgroup_struct();
    if (cgroup_data == NULL || job_info == NULL)
        return (NULL);
    get_mem_max_usage(cgroup_data, job_info, conf);
    get_oom_status(cgroup_data, job_info, conf);
    get_cpuset(cgroup_data, job_info, conf);
    write_log_to_file(conf, "Gathered cgroup data", DEBUG, 0);
    return (cgroup_data);
}