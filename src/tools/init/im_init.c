// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, much var, such init!
//___________________________________________________________________________________________________________________________________________

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "demeter.h"

//INIT CONF STRUCT
//___________________________________________________________________________________________________________________________________________

demeter_conf_t *init_conf(void)
{
    demeter_conf_t *conf;

    if (!(conf = malloc(sizeof(demeter_conf_t))))
        return (NULL);
    conf->verbose_lv = 0;
    conf->log_style = SIMPLE;
    conf->log_level = INFO;
    conf->log_file_path = strdup("/var/log/demeter.log");
    conf->slurm_log_path = strdup("/var/log/slurm/");
    conf->sys_log_path = strdup("/var/log/");
    conf->demeter_comp_loc = strdup("http://elastic:9200/demeter/_doc");
    conf->demeter_comp_proxy = NULL;
    conf->using_task_plugin = false;
    return (conf);
}

// INIT FILE FOR DEMETER LOG
//___________________________________________________________________________________________________________________________________________
FILE *init_log_file(demeter_conf_t *conf, bool silent)
{
    FILE *log_file;

    log_file = fopen(conf->log_file_path, "a");
    if (!log_file) {
        if (!silent)
            debug2("demeter: warning: can't open log file. Will try to open log file at \"/tmp/demeter.log\".");
        //substitut log file if chosen one is not writable (although this one may not be writable either)
        log_file = fopen("/tmp/demeter.log", "a");
    }
    if (!log_file) {
        //if we can't open the log file, we can't log anything
        error("ERROR: demeter: can't open log file at /tmp/demeter.log either. Exiting.");
        return (NULL);
    }
    return (log_file);
}

// INIT CGROUP STRUCT
//___________________________________________________________________________________________________________________________________________

cgroup_data_t *init_cgroup_struct(void)
{
    cgroup_data_t *cgroup_data = malloc(sizeof(cgroup_data_t));

    if (!cgroup_data)
        return (NULL);
    cgroup_data->mem_max_usage_bytes = 0;
    cgroup_data->oom_kill_disable = 0;
    cgroup_data->under_oom = 0;
    cgroup_data->oom_kill = 0;
    cgroup_data->cpuset_cpus = NULL;
    cgroup_data->cpuset_effective_cpus = NULL;
    cgroup_data->step_id = 4294967200;
    return (cgroup_data);
}

// INNIT PARSED LOG STRUCT
//___________________________________________________________________________________________________________________________________________

parsed_log_t *init_parsed_log(void)
{
    parsed_log_t *parsed_log = malloc(sizeof(parsed_log_t));

    if (!parsed_log)
        return (NULL);
    parsed_log->unparsed_log = NULL;
    parsed_log->log_proc_name = NULL;
    parsed_log->log_source_path = NULL;
    parsed_log->log_time_str = NULL;
    parsed_log->log_level = NONE;
    parsed_log->error_code = 0;
    return (parsed_log);
}
