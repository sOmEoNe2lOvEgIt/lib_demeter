// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such conf, much modularity!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include "demeter.h"
#include "src/common/slurm_xlator.h"

// CCONVERT STRING TO LOG LEVEL
//___________________________________________________________________________________________________________________________________________

static dem_log_level_t get_slurm_log_level(char *str_level)
{
    if (!str_level)
        return (INFO);
    if (strncmp(str_level, "DEBUG", 5) == 0)
        return (DEBUG);
    if (strncmp(str_level, "INFO", 4) == 0)
        return (INFO);
    if (strncmp(str_level, "WARNING", 7) == 0)
        return (WARNING);
    if (strncmp(str_level, "ERROR", 5) == 0)
        return (ERROR);
    if (strncmp(str_level, "FATAL", 5) == 0)
        return (FATAL);
    return (INFO);
}

// TRY A PATH TO A FILE AND CHECK IF WRITABLE
//___________________________________________________________________________________________________________________________________________

static bool is_conf_path_accesible(char *path)
{
    FILE *file = fopen(path, "r");
 
    if (!file)
        return (false);
    fclose(file);
    return (true);
}

// READ CONFIG FILE
//___________________________________________________________________________________________________________________________________________

demeter_conf_t *read_conf(void)
{
    s_p_options_t options[] = {
    {"Verbose", S_P_UINT32}, {"TaskPlugin", S_P_STRING},
    {"LogStyle", S_P_STRING}, {"LogLevel", S_P_STRING},
    {"SlurmLogLevel", S_P_STRING}, {"LogFilePath", S_P_STRING},
    {"SlurmLogPath", S_P_STRING}, {"SysLogPath", S_P_STRING},
    {"DemeterCompLoc", S_P_STRING}, {"DemeterCompProxy", S_P_STRING},
    {NULL}};
    demeter_conf_t *conf = init_conf();
    char *log_style = NULL, *log_level = NULL, *slurm_log_level = NULL,
    *log_file_path = NULL, *slurm_log_path = NULL, *demeter_comp_loc = NULL,
    *demeter_comp_proxy = NULL, *sys_log_path = NULL, task_plugin = NULL,
    teststr[1000], conf_path[] = "/etc/slurm/demeter.conf";
    s_p_hashtbl_t *tbl = NULL;

    memset(teststr, 0, 1000);
    tbl = s_p_hashtbl_create(options);
    if (!tbl)
        return (NULL);
    if (s_p_parse_file(tbl, NULL, conf_path, false) != SLURM_SUCCESS) {
        s_p_hashtbl_destroy(tbl);
        return (conf);
    }
    s_p_get_uint32(&conf->verbose_lv, "Verbose", tbl);
    s_p_get_string(&task_plugin, "TaskPlugin", tbl);
    s_p_get_string(&log_style, "LogStyle", tbl);
    s_p_get_string(&log_level, "LogLevel", tbl);
    s_p_get_string(&slurm_log_level, "SlurmLogLevel", tbl);
    s_p_get_string(&log_file_path, "LogFilePath", tbl);
    s_p_get_string(&slurm_log_path, "SlurmLogPath", tbl);
    s_p_get_string(&sys_log_path, "SysLogPath", tbl);
    s_p_get_string(&demeter_comp_loc, "DemeterCompLoc", tbl);
    s_p_get_string(&demeter_comp_proxy, "DemeterCompProxy", tbl);
    if (log_style) {
        if (strncmp(log_style, "FANCY", 5) == 0)
            conf->log_style = FANCY;
        if (strncmp(log_style, "SIMPLE", 6) == 0)
            conf->log_style = SIMPLE;
        if (strncmp(log_style, "SYSTEM", 6) == 0)
            conf->log_style = SYSTEM;
        xfree(log_style);
    }
    if (log_level) {
        conf->log_level = get_slurm_log_level(log_level);
        xfree(log_level);
    }
    if (slurm_log_level) {
        conf->slurm_log_level = get_slurm_log_level(slurm_log_level);
        xfree(slurm_log_level);
    }
    if (log_file_path) {
        if (is_conf_path_accesible(log_file_path)) {
            if (conf->log_file_path)
                free(conf->log_file_path);
            conf->log_file_path = strdup(log_file_path);
        }
        xfree(log_file_path);
    }
    if (slurm_log_path) {
        if (is_conf_path_accesible(slurm_log_path)) {
            if (conf->slurm_log_path)
                free(conf->slurm_log_path);
            conf->slurm_log_path = strdup(slurm_log_path);
        }
        xfree(slurm_log_path);
    }
    if (sys_log_path) {
        if (is_conf_path_accesible(sys_log_path)) {
            if (conf->sys_log_path)
                free(conf->sys_log_path);
            conf->sys_log_path = strdup(sys_log_path);
        }
        xfree(sys_log_path);
    }
    if (demeter_comp_loc) {
        if (demeter_comp_loc) {
            if (conf->demeter_comp_loc)
                free(conf->demeter_comp_loc);
            conf->demeter_comp_loc = strdup(demeter_comp_loc);
        }
        xfree(demeter_comp_loc);
    }
    if (demeter_comp_proxy) {
        if (demeter_comp_proxy) {
            if (conf->demeter_comp_proxy)
                free(conf->demeter_comp_proxy);
            conf->demeter_comp_proxy = strdup(demeter_comp_proxy);
        }
        xfree(demeter_comp_proxy);
    }
    if (task_plugin) {
        if (!strncmp(task_plugin, "true", 4))
            conf->using_task_plugin = true;
        xfree(task_plugin);
    }
    s_p_hashtbl_destroy(tbl);
    return (conf);
}