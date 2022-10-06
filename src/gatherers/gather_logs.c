// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, many info!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <syslog.h>
#include "slurm/slurm.h"
#include "demeter.h"

parsed_log_t *init_parsed_log(void)
{
    parsed_log_t *parsed_log = malloc(sizeof(parsed_log_t));

    if (parsed_log == NULL)
        return (NULL);
    parsed_log->unparsed_log = NULL;
    parsed_log->log_proc_name = NULL;
    parsed_log->log_source_path = NULL;
    parsed_log->log_time_str = NULL;
    parsed_log->log_level = NONE;
    parsed_log->kernel_log_level = KERN_NONE;
    parsed_log->error_code = 0;
    return (parsed_log);
}

log_counter_t *count_log(linked_list_t *log_list)
{
    log_counter_t *log_counter = malloc(sizeof(log_counter_t));
    parsed_log_t *parsed_log = NULL;
    linked_list_t *tmp = log_list;
    
    if (log_counter == NULL)
        return (NULL);
    memset(log_counter, 0, sizeof(log_counter_t));
    while (tmp != NULL) {
        parsed_log = (parsed_log_t *)tmp->data;
        switch (parsed_log->log_level) {
            case INFO:
                log_counter->infos++;
                break;
            case WARNING:
                log_counter->warnings++;
                break;
            case ERROR:
                log_counter->errors++;
                break;
            case FATAL:
                log_counter->fatals++;
                break;
            case DEBUG:
                log_counter->debugs++;
                break;
            default:
                break;
        }
        tmp = tmp->next;
    }
    return (log_counter);
}

kern_log_counter_t *count_kern_log(linked_list_t *log_list)
{
    kern_log_counter_t *kern_log_counter = malloc(sizeof(kern_log_counter_t));
    parsed_log_t *parsed_log = NULL;
    linked_list_t *tmp = log_list;
    
    if (kern_log_counter == NULL)
        return (NULL);
    memset(kern_log_counter, 0, sizeof(kern_log_counter_t));
    while (tmp != NULL) {
        parsed_log = (parsed_log_t *)tmp->data;
        switch (parsed_log->kernel_log_level) {
            case KERN_EMERG:
                kern_log_counter->emerg++;
                break;
            case KERN_ALERT:
                kern_log_counter->alert++;
                break;
            case KERN_CRIT:
                kern_log_counter->crit++;
                break;
            case KERN_ERR:
                kern_log_counter->err++;
                break;
            case KERN_WARNING:
                kern_log_counter->warning++;
                break;
            case KERN_NOTICE:
                kern_log_counter->notice++;
                break;
            case KERN_INFO:
                kern_log_counter->info++;
                break;
            case KERN_DEBUG:
                kern_log_counter->debug++;
                break;
            default:
                break;
        }
        tmp = tmp->next;
    }
    return (kern_log_counter);
}

linked_list_t *gather_logs(demeter_conf_t *demeter_conf, job_id_info_t *job_info, log_counter_t **log_counter, kern_log_counter_t **kern_log_counter)
{
    linked_list_t *log_list;

    log_list = gather_system_logs(demeter_conf, job_info, NULL);
    log_list = gather_slurm_logs(demeter_conf, job_info, log_list);
    *log_counter = count_log(log_list);
    *kern_log_counter = count_kern_log(log_list);
    return (log_list);
}