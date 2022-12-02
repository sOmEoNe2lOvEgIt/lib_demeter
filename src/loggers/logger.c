// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, much file, amny demeter debugging!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <inttypes.h>
#include <slurm/slurm.h>
#include <slurm/slurm_errno.h>
#include <src/common/log.h>
#include "demeter.h"

//LOGGING FUNCTIONS
//___________________________________________________________________________________________________________________________________________ 
static char *get_debug_level_str(uint verbose)
{
    char log_lev[30];

    sprintf(log_lev, "debug%u: ", verbose);
    switch (verbose) {
        case 0:
        case 1:
            return ("debug: ");
        default:
            return (strdup(log_lev));
    }
}

static char *get_log_level_str(dem_log_level_t level , uint verbose)
{
    switch (level) {
        case DEBUG:
            return (get_debug_level_str(verbose));
        case INFO:
            return (strdup("[INFO]: "));
        case WARNING:
            return (strdup("[WARN]: "));
        case ERROR:
            return (strdup("[ERROR]: "));
        case FATAL:
            return (strdup("[FATAL]: "));
        default:
            return (NULL);
    }
}

int write_log_to_file(demeter_conf_t *conf, char *message, dem_log_level_t level, uint verbose)
{
    FILE *log_file;
    char *log_level, *time;

    if (verbose > conf->verbose_lv || level < conf->log_level)
        return (0);
    log_file = init_log_file(conf, true);
    if (!log_file) {
        debug3("demeter : can't write to log file, log file is NULL.");
        return (1);
    }
    log_level = get_log_level_str(level, verbose);
    if (!log_level) {
        debug3("demeter : can't write to log file, log level is NULL.");
        return (1);
    }
    time = get_time_str();
    switch (conf->log_style) {
        case FANCY:
            fprintf(log_file, "%s:[demeter]> %s%s\n", time, log_level, message);
            break;
        case SIMPLE:
            fprintf(log_file, "%s demeter: %s%s\n", time, log_level, message);
            break;
        case SYSTEM:
            fprintf(log_file, "demeter: %s%s\n", log_level, message);
            break;
        default:
            debug3("error : invalid log format.");
            break;
    }
    fclose(log_file);
    free(time);
    free(log_level);
    return (0);
}
