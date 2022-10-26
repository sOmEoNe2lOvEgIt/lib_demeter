// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such job, much transfer!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include "slurm/slurm.h"
#include "src/common/slurm_xlator.h"
#include "demeter.h"

char *get_job_transfer_path(uint job_id)
{
    char *cgroup_basepath = NULL, *cgroup_path = NULL;
    
    cgroup_basepath = slurm_conf.slurmd_spooldir;
    if (!cgroup_basepath)
        return NULL;
    cgroup_path = malloc(sizeof(char) * (strlen(cgroup_basepath) + 29));
    sprintf(cgroup_path, "%s/demeter_job_%u", cgroup_basepath, job_id);
    return (cgroup_path);
}