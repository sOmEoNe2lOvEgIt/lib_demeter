// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, good level!
//___________________________________________________________________________________________________________________________________________

#define _GNU_SOURCE
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include "slurm/slurm.h"
#include "demeter.h"

bool handle_log_level(parsed_log_t *curr_log, demeter_conf_t *demeter_conf)
{
    dem_log_level_t level_of_curr_log = NONE;

    if (curr_log == NULL || curr_log->unparsed_log == NULL)
        return false;
    if (strcasestr(curr_log->unparsed_log, "FATAL") != NULL ||
    strcasestr(curr_log->unparsed_log, "fatal") != NULL)
        level_of_curr_log = FATAL;
    if (strcasestr(curr_log->unparsed_log, "ERROR") != NULL ||
    strcasestr(curr_log->unparsed_log, "kern err") != NULL)
        level_of_curr_log = ERROR;
    if (strcasestr(curr_log->unparsed_log, "WARNING") != NULL ||
    strcasestr(curr_log->unparsed_log, "kern warn") != NULL)
        level_of_curr_log = WARNING;
    if (strcasestr(curr_log->unparsed_log, "INFO") != NULL ||
    strcasestr(curr_log->unparsed_log, "kern info") != NULL)
        level_of_curr_log = INFO;
    if (strcasestr(curr_log->unparsed_log, "DEBUG") != NULL ||
    strcasestr(curr_log->unparsed_log, "kern debug") != NULL)
        level_of_curr_log = DEBUG;
    if (level_of_curr_log < demeter_conf->slurm_log_level &&
    level_of_curr_log != NONE) {
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
    curr_log->log_level = level_of_curr_log;
    return true;
}
