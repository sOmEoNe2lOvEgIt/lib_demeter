// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such sel, many anger!
// (this is a play on words as sel is french for salt as in salty so angry...)
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <syslog.h>
#include "slurm/slurm.h"
#include "demeter.h"

parsed_sel_t *init_parsed_sel(void)
{
    parsed_sel_t *parsed_sel = malloc(sizeof(parsed_sel_t));

    parsed_sel->unparsed_sel = NULL;
    parsed_sel->sel_time_str = NULL;
    parsed_sel->sel_msg = NULL;
    parsed_sel->sel_msg_type = NULL;
    parsed_sel->asserted = false;
    return (parsed_sel);
}

linked_list_t *gather_sel(job_id_info_t *job_info)
{
    FILE *log_fd = NULL;
    linked_list_t *sel_list = NULL;
    parsed_sel_t *curr_log = NULL;
    char *buffer = NULL;
    size_t len = 1000;

    if ((log_fd = popen("ipmitool -U admin -P password sel list", "r")) == NULL)
        return (NULL);
    sel_list = add_to_list(sel_list, init_parsed_sel());
    while (getline(&buffer, &len, log_fd) != -1) {
        curr_log = (parsed_sel_t *)sel_list->data;
        curr_log->unparsed_sel = strdup(buffer);
        if (get_sel_time(curr_log, job_info->start_time))
            continue;
        if (get_sel_element(curr_log, &curr_log->sel_msg_type, 3))
            continue;
        if (get_sel_element(curr_log, &curr_log->sel_msg, 4))
            continue;
        if (get_sel_assert(curr_log))
            continue;
        sel_list = add_to_list(sel_list, init_parsed_sel());
    }
    pclose(log_fd);
    return (sel_list);
}