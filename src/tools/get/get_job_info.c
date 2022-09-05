// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, much job, many info!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <time.h>
#include "slurm/slurm.h"
#include "demeter.h"

job_id_info_t *get_job_info(job_env_t* job)
{
    job_id_info_t *job_info = malloc(sizeof(job_id_info_t));

    if (job_info == NULL) {
        my_slurm_error("malloc failed");
        return (NULL);
    }
    job_info->job_id = job->jobid;
    job_info->uid = job->uid;
    job_info->step_id = job->step_id;
    time(&job_info->start_time);
    return (job_info);
}
