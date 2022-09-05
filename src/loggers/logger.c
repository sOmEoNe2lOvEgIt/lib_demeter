// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, much file!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <inttypes.h>
#include "slurm/slurm.h"
#include "slurm/slurm_errno.h"
#include "src/common/log.h"
#include "demeter.h"

// LOGGING TOOLS
//___________________________________________________________________________________________________________________________________________
FILE *init_log_file(demeter_conf_t *conf, bool silent)
{
    FILE *log_file;

    log_file = fopen(conf->log_file_path, "a");
    if (log_file == NULL) {
        if (!silent)
            debug2("demeter: warning: can't open log file. Will try to open log file at \"/tmp/demeter.log\".");
        //substitut log file if chosen one is not writable (although this one may not be writable either)
        log_file = fopen("/tmp/demeter.log", "a");
    }
    if (log_file == NULL) {
        //if we can't open the log file, we can't log anything
        error("ERROR: demeter: can't open log file at /tmp/demeter.log either. Exiting.");
        return (NULL);
    }
    return (log_file);
}

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
    char *log_level;

    if (verbose > conf->verbose_lv || level < conf->log_level)
        return (0);
    log_file = init_log_file(conf, true);
    if (log_file == NULL) {
        debug3("demeter : can't write to log file, log file is NULL.");
        return (1);
    }
    log_level = get_log_level_str(level, verbose);
    if (log_level == NULL) {
        debug3("demeter : can't write to log file, log level is NULL.");
        return (1);
    }
    switch (conf->log_style) {
        case FANCY:
            fprintf(log_file, "%s:[demeter]> %s%s\n", get_time_str(), log_level, message);
            break;
        case SIMPLE:
            fprintf(log_file, "%s demeter: %s%s\n", get_time_str(), log_level, message);
            break;
        case SYSTEM:
            fprintf(log_file, "demeter: %s%s\n", log_level, message);
            break;
        default:
            debug3("error : invalid log format.");
            fclose(log_file);
    }
    fclose(log_file);
    free(log_level);
    return (0);
}
