// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such cgroup, much logging!
//___________________________________________________________________________________________________________________________________________

#include <string.h>
#include "src/slurmd/slurmd/slurmd.h"
#include "demeter.h"

void log_cgroup(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf)
{
    char log_msg[250];

    if (cgroup_data == NULL || job_info == NULL || conf == NULL)
        return;
    if (job_info->step_id >= 4294967292)
        sprintf(log_msg, "\nCgroup data for job %u from user %u, after last_step:\n\tmem_max_usage_bytes: %u\n\toom_kill_disable: %d\n\tunder_oom: %d\n\toom_kill: %d\n\tcpuset_cpus: %s\n\tcpuset_effective_cpus: %s",
        job_info->job_id, job_info->uid, cgroup_data->mem_max_usage_bytes, cgroup_data->oom_kill_disable, cgroup_data->under_oom, cgroup_data->oom_kill, cgroup_data->cpuset_cpus, cgroup_data->cpuset_effective_cpus);
    else
        sprintf(log_msg, "\nCgroup data for job %u from user %u, step %u:\n\tmem_max_usage_bytes: %u\n\toom_kill_disable: %d\n\tunder_oom: %d\n\toom_kill: %d\n\tcpuset_cpus: %s\n\tcpuset_effective_cpus: %s",
        job_info->job_id, job_info->uid, job_info->step_id, cgroup_data->mem_max_usage_bytes, cgroup_data->oom_kill_disable, cgroup_data->under_oom, cgroup_data->oom_kill, cgroup_data->cpuset_cpus, cgroup_data->cpuset_effective_cpus);
    write_log_to_file(conf, log_msg, INFO, 0);
}
