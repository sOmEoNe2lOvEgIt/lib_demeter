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
    if (strstr(curr_log->unparsed_log, "FATAL") != NULL || strstr(curr_log->unparsed_log, "fatal") != NULL)
        level_of_curr_log = FATAL;
    if (strstr(curr_log->unparsed_log, "ERROR") != NULL || strstr(curr_log->unparsed_log, "error") != NULL)
        level_of_curr_log = ERROR;
    if (strstr(curr_log->unparsed_log, "WARNING") != NULL || strstr(curr_log->unparsed_log, "warning") != NULL)
        level_of_curr_log = WARNING;
    if (strstr(curr_log->unparsed_log, "INFO") != NULL || strstr(curr_log->unparsed_log, "info") != NULL)
        level_of_curr_log = INFO;
    if (strstr(curr_log->unparsed_log, "DEBUG") != NULL || strstr(curr_log->unparsed_log, "debug") != NULL)
        level_of_curr_log = DEBUG;
    if (level_of_curr_log < demeter_conf->slurm_log_level) {
        if (curr_log->unparsed_log != NULL) {
            free(curr_log->unparsed_log);
            curr_log->unparsed_log = NULL;
        }
        if (curr_log->log_source_path != NULL) {
            free(curr_log->log_source_path);
            curr_log->log_source_path = NULL;
        }
        return false;
    }
    return true;
}