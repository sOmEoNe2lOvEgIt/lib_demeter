// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, good level!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <syslog.h>
#include "slurm/slurm.h"
#include "demeter.h"

bool handle_log_level(parsed_log_t *curr_log, demeter_conf_t *demeter_conf)
{
    dem_log_level_t level_of_curr_log = DEBUG;

    if (curr_log->unparsed_log == NULL)
        return false;
    if (strstr(curr_log->unparsed_log, "FATAL") != NULL)
        level_of_curr_log = FATAL;
    if (strstr(curr_log->unparsed_log, "ERROR") != NULL)
        level_of_curr_log = ERROR;
    if (strstr(curr_log->unparsed_log, "WARNING") != NULL)
        level_of_curr_log = WARNING;
    if (strstr(curr_log->unparsed_log, "INFO") != NULL)
        level_of_curr_log = INFO;
    if (strstr(curr_log->unparsed_log, "DEBUG") != NULL)
        level_of_curr_log = DEBUG;
    if (level_of_curr_log < demeter_conf->slurm_log_level) {
        if (!is_log_empty(curr_log->unparsed_log)) {
            free(curr_log->unparsed_log);
            curr_log->unparsed_log = NULL;
        }
        if (!is_log_empty(curr_log->log_source_path)) {
            free(curr_log->log_source_path);
            curr_log->log_source_path = NULL;
        }
        return false;
    }
    curr_log->log_level = level_of_curr_log;
    return true;
}

bool handle_sys_log_level(parsed_log_t *curr_log, demeter_conf_t *demeter_conf)
{
    dem_log_level_t level_of_curr_log = DEBUG;

    if (curr_log->unparsed_log == NULL)
        return false;
    if (strstr(curr_log->unparsed_log, "FATAL") != NULL)
        level_of_curr_log = FATAL;
    if (strstr(curr_log->unparsed_log, "ERROR") != NULL)
        level_of_curr_log = ERROR;
    if (strstr(curr_log->unparsed_log, "WARNING") != NULL)
        level_of_curr_log = WARNING;
    if (strstr(curr_log->unparsed_log, "INFO") != NULL)
        level_of_curr_log = INFO;
    if (strstr(curr_log->unparsed_log, "DEBUG") != NULL)
        level_of_curr_log = DEBUG;
    if (level_of_curr_log < demeter_conf->sys_log_level) {
        if (!is_log_empty(curr_log->unparsed_log)) {
            free(curr_log->unparsed_log);
            curr_log->unparsed_log = NULL;
        }
        if (!is_log_empty(curr_log->log_source_path)) {
            free(curr_log->log_source_path);
            curr_log->log_source_path = NULL;
        }
        return false;
    }
    curr_log->log_level = level_of_curr_log;
    return true;
}