// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such conf, much modularity!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include "demeter.h"
#include "src/common/slurm_xlator.h"

static char *get_hostname(void)
{
    char *hostname = NULL;
    char *line = NULL;
    FILE *hostname_file = NULL;
    size_t hostname_len = 100;

    if ((hostname_file = fopen("/etc/hostname", "r")) == NULL)
        return (NULL);
    getline(&line, &hostname_len, hostname_file);
    fclose(hostname_file);
    hostname_len = get_len_to_char(line, '\n');
    line[hostname_len] = '\0';
    hostname = strdup(line);
    if (line != NULL)
        free(line);
    else
        return (NULL);
    return (hostname);
}

static demeter_conf_t *init_conf(void)
{
    demeter_conf_t *conf;
    char *hostname = NULL;
    char slurm_log_path[80];

    memset(slurm_log_path, 0, 80);
    if ((hostname = get_hostname()) == NULL)
        return (NULL);
    if ((conf = malloc(sizeof(demeter_conf_t))) == NULL) {
        free(hostname);
        return (NULL);
    }
    sprintf(slurm_log_path, "/var/log/slurm/slurm.%s.log", hostname);
    conf->verbose_lv = 0;
    conf->log_style = SIMPLE;
    conf->log_level = INFO;
    conf->log_file_path = strdup("/var/log/demeter.log");
    conf->slurm_log_path = strdup(slurm_log_path);
    free(hostname);
    return (conf);
}

static dem_log_level_t get_slurm_log_level(char *str_level)
{
    if (str_level == NULL)
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

static bool is_conf_path_accesible(char *path)
{
    FILE *file = fopen(path, "r");
 
    if (file == NULL)
        return (false);
    fclose(file);
    return (true);
}

demeter_conf_t *read_conf(void)
{
    s_p_options_t options[] = {{"Verbose", S_P_UINT32},
    {"LogStyle", S_P_STRING}, {"LogLevel", S_P_STRING},
    {"SlurmLogLevel", S_P_STRING}, {"SysLogLevel", S_P_STRING},
    {"LogFilePath", S_P_STRING}, {"SlurmLogPath", S_P_STRING}, {NULL}};
    demeter_conf_t *conf = init_conf();
    char *log_style = NULL, *log_level = NULL, *slurm_log_level = NULL,
    *sys_log_level = NULL, *log_file_path = NULL, *slurm_log_path = NULL,
    teststr[1000], conf_path[] = "/etc/slurm/demeter.conf";
    s_p_hashtbl_t *tbl = NULL;

    memset(teststr, 0, 1000);
    tbl = s_p_hashtbl_create(options);
    if (s_p_parse_file(tbl, NULL, conf_path, false) == SLURM_ERROR) {
        s_p_hashtbl_destroy(tbl);
        return (conf);
    }
    s_p_get_uint32(&conf->verbose_lv, "Verbose", tbl);
    s_p_get_string(&log_style, "LogStyle", tbl);
    s_p_get_string(&log_level, "LogLevel", tbl);
    s_p_get_string(&slurm_log_level, "SlurmLogLevel", tbl);
    s_p_get_string(&sys_log_level, "SysLogLevel", tbl);
    s_p_get_string(&log_file_path, "LogFilePath", tbl);
    s_p_get_string(&slurm_log_path, "SlurmLogPath", tbl);
    if (log_style != NULL) {
        if (strncmp(log_style, "FANCY", 5) == 0)
            conf->log_style = FANCY;
        if (strncmp(log_style, "SIMPLE", 6) == 0)
            conf->log_style = SIMPLE;
        if (strncmp(log_style, "SYSTEM", 6) == 0)
            conf->log_style = SYSTEM;
    }
    if (log_level != NULL) {
        conf->log_level = get_slurm_log_level(log_level);
        xfree(log_level);
    }
    if (slurm_log_level != NULL) {
        conf->slurm_log_level = get_slurm_log_level(slurm_log_level);
        xfree(slurm_log_level);
    }
    if (sys_log_level != NULL) {
        conf->sys_log_level = get_slurm_log_level(sys_log_level);
        xfree(sys_log_level);
    }
    if (log_file_path != NULL) {
        if (is_conf_path_accesible(log_file_path))
            conf->log_file_path = strdup(log_file_path);
        xfree(log_file_path);
    }
    if (slurm_log_path != NULL) {
        if (is_conf_path_accesible(slurm_log_path))
            conf->slurm_log_path = strdup(slurm_log_path);
        xfree(slurm_log_path);
    }
    s_p_hashtbl_destroy(tbl);
    sprintf(teststr, "%u,%u,%s,%s", conf->verbose_lv, conf->log_style, conf->log_file_path, conf->slurm_log_path);
    write_log_to_file(conf, teststr, DEBUG, 99);
    return (conf);
}