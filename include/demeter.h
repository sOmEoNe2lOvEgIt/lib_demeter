// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such include, many things!
//___________________________________________________________________________________________________________________________________________

#include <inttypes.h>
#include <string.h>
#include <zlib.h>
#include "slurm/slurm.h"
#include "src/slurmd/slurmd/slurmd.h"
#include "src/slurmd/slurmstepd/slurmstepd_job.h"
#include "gather_ib.h"

#ifndef DEMETER_H_
#define DEMETER_H_

// ENUMS&STRUCTS
//___________________________________________________________________________________________________________________________________________

typedef enum log_style_e {
    FANCY=0,
    SIMPLE=1,
    SYSTEM=2,
} log_style_t;
// Log style for demeter log output.

typedef enum dem_log_level_e {
    DEBUG=0,
    INFO=1,
    WARNING=2,
    ERROR=3,
    FATAL=4,
    NONE=5,
} dem_log_level_t;
// Log level for demeter log output.

typedef struct demeter_conf_s {
    uint verbose_lv;
    // Verbosity level if log level is DEBUG.
    dem_log_level_t log_level;
    // Log level for demeter log output.
    log_style_t log_style;
    char *log_file_path;
    // Path to demeter log file.
    char *slurm_log_path;
    // Path to file in which demeter will parse log from.
    char *sys_log_path;
    // Path to file in which demeter will parse log from.
    dem_log_level_t slurm_log_level;
    // Log level for slurm log parsing.
    char *demeter_comp_loc;
    // Location of elasticsearch database.
    char *demeter_comp_proxy;
    // Proxy to use for elasticsearch database.
} demeter_conf_t;
// Demeter configuration.

typedef struct cgroup_data_s {
    uint mem_max_usage_bytes;
    // Maximum memory usage in bytes.
    uint oom_kill_disable;
    uint under_oom;
    uint oom_kill; 
    char *cpuset_cpus;
    char *cpuset_effective_cpus;
    uint step_id;
} cgroup_data_t;
// Cgroup gathered data for each job step || job.

typedef struct job_id_info_s {
    uint job_id;
    // Id from current job when setup propperly.
    uint uid;
    // User id from current job when setup propperly.
    uint step_id;
    // Id from current job step when setup propperly.
    time_t start_time;
    // "Start time for job", actually time of
    //execution of the acct_gather_profile_p_node_step_start function in the plugin.
    // Used to verify that logs gathered are logs that are timestamped in job runtime.
} job_id_info_t;

typedef struct parsed_log_s {
    char *unparsed_log;
    // Raw log line.
    char *log_proc_name;
    // Name of process outputing log.
    char *log_source_path;
    //path to the log file, "stdout" if stdout
    char *log_time_str;
    // Time & date of log in readable format.
    //struct host_info_s *host_info; <-- future struct whith info about the host to implement eventually
    int error_code;
    //0 if no error, 1 if error, only used for stdout as log source
    dem_log_level_t log_level;
    // Log level of log.
} parsed_log_t;
// Logs gathered for each job step || job.

typedef struct parsed_sel_s {
    char *unparsed_sel;
    // Raw sel line.
    char *sel_time_str;
    // Time & date of sel in readable format.
    char *sel_msg_type;
    // Type of sel message.
    char *sel_msg;
    // Actual sel message.
    bool asserted;
    // True if sel message is "asserted".
    dem_log_level_t log_level;
    // Log level of log.
} parsed_sel_t;

typedef struct linked_list_s {
    void *data;
    // Needs to be casted to the appropriate type.
    struct linked_list_s *next;
} linked_list_t;
// Generic linked list.

typedef struct log_counter_s {
    uint errors;
    uint warnings;
    uint infos;
    uint debugs;
    uint fatals;
} log_counter_t;


// TOOLS
//___________________________________________________________________________________________________________________________________________

char *get_time_str(void);
// Returns time in readable format.
int get_len_to_char(char *str, char c);
// Returns distance to the next char c in  string str.
char *get_job_transfer_path(uint job_id);
char *get_hostname(void);
int zgetline(char **line, gzFile file);
// Returns a line from a gzFile.
ssize_t getline_from_end(char **line, size_t *len, FILE *file, long *line_offset);
// Returns line from end of file. Offset of Line_nb
ssize_t zgetline_from_end(char **line, size_t *len, gzFile file, long *line_offset);
// Returns line from end of gzfile. Offset of Line_nb
bool is_log_empty(char *log);
// Returns true if log is empty.
demeter_conf_t *read_conf(void);
// Reads demeter configuration.
job_id_info_t *get_job_info(job_env_t* job);
// Returns job info.
linked_list_t *add_to_list(linked_list_t *list, void *data);
// Adds a new link to the begining
//of the list given as arg, returns new said link.
bool handle_log_level(parsed_log_t *curr_log, demeter_conf_t *demeter_conf);
// Handles log level.
void remove_newline(char *str);
// Removes newline from string.
char *append_str(char *str, char *to_append);
// Appends string to another.
time_t get_rotate_time(char *file_name);
// Returns time of last rotation of file. (file name has to have "~log-YYYY-MM-DD~" format)

// I'M FREE!!!
//___________________________________________________________________________________________________________________________________________

// Functions for freeing different types of structs whithin demeter.
void free_list(linked_list_t *list);
void free_conf(demeter_conf_t *conf);
void free_cgroup_list(linked_list_t *cgroup_list);
void free_cgroup(cgroup_data_t *data);
void free_job_id_info(job_id_info_t *job_info);
void free_parsed_log(parsed_log_t *log);
void free_log_list(linked_list_t *log_list);
void free_log_counter(log_counter_t *log_counter);
void free_sel_list(linked_list_t *sel_list);
void free_parsed_sel(parsed_sel_t *parsed_sel);
void free_perf_count(perf_data_t *perf_count);

// LOGGER FUNCTIONS
//___________________________________________________________________________________________________________________________________________

FILE *init_log_file(demeter_conf_t *conf, bool silent);
// Initializes log file.
int write_log_to_file(demeter_conf_t *conf, char *message, dem_log_level_t level, uint verbose);
// Writes log to demeter log file.
void log_parsed_logs(linked_list_t *gathered_logs, demeter_conf_t *demeter_conf);
// Specific to gathered logs.
void log_cgroup(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);
// Specific to cgroup data.
void transfer_log_cgroup(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);
// for transfering cgroup data between plugins.
void log_parsed_sel(linked_list_t *gathered_sel);
// Specific to gathered sel.
int send_elastic(demeter_conf_t *demeter_conf ,job_id_info_t *job_info, linked_list_t *cgroup_data,
linked_list_t *gathered_logs, log_counter_t *log_counter,
linked_list_t *gathered_sel, perf_data_t *gathered_perf_data);

// JSON FORMATTERS
//__________________________________________________________________________________________________________________________________________

char *format_logs(linked_list_t *gathered_logs, linked_list_t *gathered_sel);
char *format_cgroup(linked_list_t *cgroup);
char *format_job_info(job_id_info_t *job_info);
char *format_log_counter(log_counter_t *log_counter);
char *format_perf_count(perf_data_t *perf_data);

// CGROUP FUNCTION
//___________________________________________________________________________________________________________________________________________

cgroup_data_t *gather_cgroup(job_id_info_t *job_info, demeter_conf_t *conf);
// Gathers cgroup data.
linked_list_t *unlog_cgroup(demeter_conf_t *demeter_conf, uint job_id);

// CGROUP TOOLS
//___________________________________________________________________________________________________________________________________________

cgroup_data_t *alloc_cgroup_struct(void);
void get_oom_status(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);
void get_mem_max_usage(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);
void get_cpuset(cgroup_data_t *cgroup_data, job_id_info_t *job_info, demeter_conf_t *conf);

// LOG_PARSER FUNCTION
//___________________________________________________________________________________________________________________________________________

linked_list_t *gather_logs(demeter_conf_t *demeter_conf, job_id_info_t *job_info, log_counter_t **log_counter);
// Gathers logs.

// LOG_PARSER TOOLS
//___________________________________________________________________________________________________________________________________________

parsed_log_t *init_parsed_log(void);
// Returns a new propperly allocated empty parsed log struct.
int get_sys_log_time(parsed_log_t *log_to_parse, time_t start_time);
// Adds readable time to log. Returns 0 if no error,
// other if error (including the log time not being at job runtime).
int get_slurm_log_time(parsed_log_t *log_to_parse, time_t start_time);
// Adds readable time to log. Returns 0 if no error,
// positive int if error (including the log time not being at job runtime).

linked_list_t *gather_system_logs (demeter_conf_t *demeter_conf, job_id_info_t *job_info, linked_list_t *log_list);
// Gathers system logs by reading them from the syslog files and rotated files if necessary.
gzFile open_rotated_system_log(demeter_conf_t *demeter_conf, job_id_info_t *job_info);    //
FILE *open_system_logs(demeter_conf_t *demeter_conf);                                     //
// sys log tools__________________________________________________________________________//

linked_list_t *gather_slurm_logs (demeter_conf_t *demeter_conf, job_id_info_t *job_info, linked_list_t *log_list);
// Gathers slurm logs by reading them from the slurm logs files and rotated files if necessary.
gzFile open_rotated_slurm_log(demeter_conf_t *demeter_conf, job_id_info_t *job_info);      //
FILE *open_slurm_log(demeter_conf_t *demeter_conf);                                        //
// slurm log tools_________________________________________________________________________//

// SEL_PARSER FUNCTION
//___________________________________________________________________________________________________________________________________________

linked_list_t *gather_sel(job_id_info_t *job_info);

// SEL_PARSER TOOLS
//___________________________________________________________________________________________________________________________________________

int get_sel_assert(parsed_sel_t *curr_sel);
int get_sel_element(parsed_sel_t *curr_sel, char **element, int element_nb);
int get_sel_time(parsed_sel_t *curr_sel, time_t start_time);

// GATHER_IB FUNCTION
//___________________________________________________________________________________________________________________________________________

perf_data_t *gather_ib(void);
// Gathers IB data.
perf_data_t *gather_ib_diff(perf_data_t *prolog_perf_count);
// Gathers IB data and substracts prolog data.



#endif /* !DEMETER_H_ */
