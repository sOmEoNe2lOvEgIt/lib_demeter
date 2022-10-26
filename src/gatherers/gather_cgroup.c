// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such cgroup, much gathering!
//___________________________________________________________________________________________________________________________________________

#include <string.h>
#include "src/slurmd/slurmd/slurmd.h"
#include "demeter.h"

cgroup_data_t *gather_cgroup(job_id_info_t *job_info, demeter_conf_t *conf)
{
    cgroup_data_t *cgroup_data = init_cgroup_struct();

    if (!cgroup_data || !job_info)
        return (NULL);
    get_mem_max_usage(cgroup_data, job_info, conf);
    get_oom_status(cgroup_data, job_info, conf);
    get_cpuset(cgroup_data, job_info, conf);
    write_log_to_file(conf, "Gathered cgroup data", DEBUG, 0);
    return (cgroup_data);
}
