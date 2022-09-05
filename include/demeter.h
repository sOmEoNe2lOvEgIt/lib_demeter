// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such include, many things!
//___________________________________________________________________________________________________________________________________________

#include "slurm/slurm.h"
#include "src/slurmd/slurmd/slurmd.h"
#include "src/slurmd/slurmstepd/slurmstepd_job.h"
#include <inttypes.h>
#include <string.h>

#ifndef DEMETER_H_
#define DEMETER_H_

// ENUMS&STRUCTS
//___________________________________________________________________________________________________________________________________________

typedef enum log_style_e { // Log style for demeter log output.
    FANCY=0,
    SIMPLE=1,
    SYSTEM=2,
} log_style_t;

typedef enum dem_log_level_e { // Log level for demeter log output.
    DEBUG=0,
    INFO=1,
    WARNING=2,
    ERROR=3,
    FATAL=4,
} dem_log_level_t;

typedef struct demeter_conf_s { // Demeter configuration.
    uint verbose_lv; // Verbosity level if log level is DEBUG.
    dem_log_level_t log_level; // Log level for demeter log output.
    log_style_t log_style;
    char *log_file_path; // Path to demeter log file.
    char *slurm_log_path; // Path to file in which demeter will parse log from. (to be implemented)
    dem_log_level_t slurm_log_level; // Log level for slurm log parsing.
    dem_log_level_t sys_log_level; // Log level for system log parsing.
} demeter_conf_t;

typedef struct cgroup_data_s { // Cgroup gathered data for each job step || job.
    uint mem_max_usage_bytes; // Maximum memory usage in bytes.
    uint oom_kill_disable;
    uint under_oom;
    uint oom_kill; 
    char *cpuset_cpus;
    char *cpuset_effective_cpus;
} cgroup_data_t;

typedef struct job_id_info_s {
    uint job_id; // Id from current job when setup propperly.
    uint uid; // User id from current job when setup propperly.
    uint step_id; // Id from current job step when setup propperly.
    time_t start_time; // "Start time for job", actually time of
    //execution of the acct_gather_profile_p_node_step_start function in the plugin.
    // Used to verify that logs gathered are logs that are timestamped in job runtime.
} job_id_info_t;

typedef struct parsed_log_s { // Logs gathered for each job step || job.
    char *unparsed_log; // Raw log line.
    char *log_proc_name; // Name of process outputing log.
    char *log_source_path; //path to the log file, "stdout" if stdout
    char *log_time_str; // Time & date of log in readable format.
    //struct host_info_s *host_info; <-- future struct whith info about the host to implement eventually
    int error_code; //0 if no error, 1 if error, only used for stdout as log source
} parsed_log_t;

typedef struct parsed_sel_s {
    char *unparsed_sel; // Raw sel line.
    char *sel_time_str; // Time & date of sel in readable format.
    char *sel_msg_type; // Type of sel message.
    char *sel_msg; // Actual sel message.
    bool asserted; // True if sel message is "asserted".
} parsed_sel_t;

typedef struct linked_list_s { // Generic linked list.
    void *data; // Needs to be casted to the appropriate type.
    struct linked_list_s *next;
} linked_list_t;

// TOOLS
//___________________________________________________________________________________________________________________________________________

void my_slurm_info(char *message); // Prints INFO message to slurm log.
void my_slurm_error(char *message); // Prints ERROR message to slurm log.
void my_slurm_debug(char *message, int level); // Prints DEBUG message to slurm log whith log level.
char *get_time_str(void); // Returns time in readable format.
bool is_log_empty(char *log); // Returns true if log is empty.
demeter_conf_t *read_conf(void); // Reads demeter configuration.
job_id_info_t *get_job_info(job_env_t* job); // Returns job info.
linked_list_t *add_to_list(linked_list_t *list, void *data); // Adds a new link to the begining
//of the list given as arg, returns new said link.
bool handle_log_level(parsed_log_t *curr_log, demeter_conf_t *demeter_conf); // Handles log level.

// I'M FREE!!!
//___________________________________________________________________________________________________________________________________________

// Functions for freeing different types of structs whithin demeter.
void free_list(linked_list_t *list);
void free_conf(demeter_conf_t *conf);
void free_cgroup(cgroup_data_t *data);
void free_job_id_info(job_id_info_t *job_info);
void free_parsed_log(parsed_log_t *log);
void free_log_list(linked_list_t *log_list);
void free_sel_list(linked_list_t *sel_list);
void free_parsed_sel(parsed_sel_t *parsed_sel);

// LOGGER FUNCTIONS
//___________________________________________________________________________________________________________________________________________

FILE *init_log_file(demeter_conf_t *conf, bool silent); // Initializes demeter log file.
int write_log_to_file(demeter_conf_t *conf, char *message, dem_log_level_t level, uint verbose); // Writes log to demeter log file.
void log_parsed_logs(linked_list_t *gathered_logs, demeter_conf_t *demeter_conf); // Specific to gathered logs.
void log_cgroup(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf); // Specific to cgroup data.
void log_parsed_sel(linked_list_t *gathered_sel, demeter_conf_t *demeter_conf); // Specific to gathered sel.
// CGROUP FUNCTION
//___________________________________________________________________________________________________________________________________________

cgroup_data_t *gather_cgroup(job_id_info_t *job_info, demeter_conf_t *conf); // Gathers cgroup data.

// CGROUP TOOLS
//___________________________________________________________________________________________________________________________________________

void get_oom_status(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);
void get_mem_max_usage(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);
void get_cpuset(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);

// LOG_PARSER FUNCTION
//___________________________________________________________________________________________________________________________________________

linked_list_t *gather_logs(demeter_conf_t *demeter_conf, job_id_info_t *job_info, cgroup_data_t *cgroup_data); // Gathers logs.

// LOG_PARSER TOOLS
//___________________________________________________________________________________________________________________________________________

char *read_sys_logs(void); //Returns logs from kernel logs.
parsed_log_t *init_parsed_log(void) ; // Returns a new propperly allocated empty parsed log struct.
int get_sys_log_time(parsed_log_t *log_to_parse, time_t start_time); // Adds readable time to log. Returns 0 if no error,
// other if error (including the log time not being at job runtime).
int get_slurm_log_time(parsed_log_t *log_to_parse, time_t start_time); // Adds readable time to log. Returns 0 if no error,
// other if error (including the log time not being at job runtime).
linked_list_t *gather_kernel_logs (demeter_conf_t *demeter_conf, job_id_info_t *job_info, cgroup_data_t *cgroup_data, linked_list_t *log_list);
linked_list_t *gather_slurm_logs (demeter_conf_t *demeter_conf, job_id_info_t *job_info,cgroup_data_t *cgroup_data, linked_list_t *log_list);

// SEL_PARSER FUNCTION
//___________________________________________________________________________________________________________________________________________

linked_list_t *gather_sel(demeter_conf_t *demeter_conf, job_id_info_t *job_info, cgroup_data_t *cgroup_data); // Gathers sel.

// SEL_PARSER TOOLS
//___________________________________________________________________________________________________________________________________________

parsed_sel_t *init_parsed_sel(void); // Returns a new propperly allocated empty parsed sel struct.
linked_list_t *gather_sel_logs(demeter_conf_t *demeter_conf, job_id_info_t *job_info, linked_list_t *sel_list);
int handle_sel_time(parsed_sel_t *curr_sel, demeter_conf_t *demeter_conf, time_t start_time);
int handle_sel_type(parsed_sel_t *curr_sel);
int handle_sel_msg(parsed_sel_t *curr_sel);
int handle_sel_assert(parsed_sel_t *curr_sel);

#endif /* !DEMETER_H_ */
