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

    if (!curr_log || !curr_log->unparsed_log)
        return false;
    if (strcasestr(curr_log->unparsed_log, "FATAL") ||
    strcasestr(curr_log->unparsed_log, "fatal"))
        level_of_curr_log = FATAL;
    if (strcasestr(curr_log->unparsed_log, "ERROR") ||
    strcasestr(curr_log->unparsed_log, "kern err"))
        level_of_curr_log = ERROR;
    if (strcasestr(curr_log->unparsed_log, "WARNING") ||
    strcasestr(curr_log->unparsed_log, "kern warn"))
        level_of_curr_log = WARNING;
    if (strcasestr(curr_log->unparsed_log, "INFO") ||
    strcasestr(curr_log->unparsed_log, "kern info"))
        level_of_curr_log = INFO;
    if (strcasestr(curr_log->unparsed_log, "DEBUG") ||
    strcasestr(curr_log->unparsed_log, "kern debug"))
        level_of_curr_log = DEBUG;
    if (level_of_curr_log < demeter_conf->slurm_log_level &&
    level_of_curr_log != NONE) {
        if (curr_log->unparsed_log) {
            free(curr_log->unparsed_log);
            curr_log->unparsed_log = NULL;
        }
        if (curr_log->log_source_path) {
            free(curr_log->log_source_path);
            curr_log->log_source_path = NULL;
        }
        return false;
    }
    curr_log->log_level = level_of_curr_log;
    return true;
}
