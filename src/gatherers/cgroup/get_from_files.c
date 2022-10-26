// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such cgroup, much gathering!
//___________________________________________________________________________________________________________________________________________

#include <string.h>
#include <sys/eventfd.h>
#include "src/slurmd/slurmd/slurmd.h"
#include "src/common/xmalloc.h"
#include "demeter.h"

static bool is_file_opened(FILE *file, demeter_conf_t *conf)
{
    if (!file) {
        write_log_to_file(conf, "Could not open cgroup file", DEBUG, 0);
        return false;
    }
    return true;
}

void get_mem_max_usage(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf)
{
    char res[50], cgroup_path[130];
    FILE *file = NULL;

    memset(cgroup_path, 0, 130);
    memset(res, 0, 50);
    if (job_info->step_id < 4294967292)
        sprintf(cgroup_path, "/sys/fs/cgroup/memory/slurm/uid_%u/job_%u/step_%u/memory.max_usage_in_bytes", job_info->uid, job_info->job_id, job_info->step_id);
    else
        sprintf(cgroup_path, "/sys/fs/cgroup/memory/slurm/uid_%u/job_%u/memory.max_usage_in_bytes", job_info->uid, job_info->job_id);
    file = fopen(cgroup_path, "r");
    if (!is_file_opened(file, conf))
        return;
    write_log_to_file(conf, "Getting max memory usage", DEBUG, 99);
    fgets(res, 50, file);
    cgroup_data->mem_max_usage_bytes = atoi(res);
    fclose(file);
}

void get_oom_status(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf)
{
    char *res = NULL, cgroup_path[130];
    size_t read_size = 30;
    FILE *file = NULL;

    memset(cgroup_path, 0, 130);
    if (job_info->step_id < 4294967292)
        sprintf(cgroup_path, "/sys/fs/cgroup/memory/slurm/uid_%u/job_%u/step_%u/memory.oom_control", job_info->uid, job_info->job_id, job_info->step_id);
    else
        sprintf(cgroup_path, "/sys/fs/cgroup/memory/slurm/uid_%u/job_%u/memory.oom_control", job_info->uid, job_info->job_id);
    file = fopen(cgroup_path, "r");
    if (!is_file_opened(file, conf))
        return;
    write_log_to_file(conf, "Getting oom status", DEBUG, 99);
    getline(&res, &read_size, file);
    cgroup_data->oom_kill_disable = atoi(&res[17]);
    getline(&res, &read_size, file);
    cgroup_data->under_oom = atoi(&res[10]);
    getline(&res, &read_size, file);
    cgroup_data->oom_kill = atoi(&res[9]);
    fclose(file);
    free(res);
}

void get_cpuset(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf)
{
    char *res = NULL, cgroup_path[130];
    size_t read_size = 30;
    FILE *file = NULL;

    memset(cgroup_path, 0, 130);
    if (job_info->step_id < 4294967292)
        sprintf(cgroup_path, "/sys/fs/cgroup/cpuset/slurm/uid_%u/job_%u/step_%u/cpuset.cpu_exclusive", job_info->uid, job_info->job_id, job_info->step_id);
    else
        sprintf(cgroup_path, "/sys/fs/cgroup/cpuset/slurm/uid_%u/job_%u/cpuset.cpu_exclusive", job_info->uid, job_info->job_id);
    file = fopen(cgroup_path, "r");
    if (!is_file_opened(file, conf))
        return;
    write_log_to_file(conf, "Getting cpuset", DEBUG, 99);
    getline(&res, &read_size, file);
    remove_newline(res);
    cgroup_data->cpuset_cpus = strdup(res);
    fclose(file);
    if (job_info->step_id < 4294967292)
        sprintf(cgroup_path, "/sys/fs/cgroup/cpuset/slurm/uid_%u/job_%u/step_%u/cpuset.effective_cpus", job_info->uid, job_info->job_id, job_info->step_id);
    else
        sprintf(cgroup_path, "/sys/fs/cgroup/cpuset/slurm/uid_%u/job_%u/cpuset.effective_cpus", job_info->uid, job_info->job_id);
    file = fopen(cgroup_path, "r");
    if (!is_file_opened(file, conf))
        return;
    write_log_to_file(conf, "Getting effective cpus", DEBUG, 99);
    getline(&res, &read_size, file);
    remove_newline(res);
    cgroup_data->cpuset_effective_cpus = strdup(res);
    fclose(file);
    free(res);
}