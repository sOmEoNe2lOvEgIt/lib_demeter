// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such sel, many anger, great logs!
// (this is a play on words as sel is french for salt as in salty so angry...)
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include <time.h>
#include "slurm/slurm.h"
#include "demeter.h"

linked_list_t *gather_sel_logs
(demeter_conf_t *demeter_conf, job_id_info_t *job_info, linked_list_t *sel_list)
{
    FILE *log_file = NULL;
    parsed_sel_t *curr_log = NULL;
    char *buffer = NULL;
    size_t len = 1000;

    if ((log_file = fopen("/shared/bmc.sel.list.taranis1061", "r")) == NULL) {
        write_log_to_file(demeter_conf, "cannot exec the sel logs command", DEBUG, 3);
        return (NULL);
    }
    sel_list = add_to_list(sel_list, init_parsed_sel());
    while (getline(&buffer, &len, log_file) != -1) {
        curr_log = (parsed_sel_t *)sel_list->data;
        curr_log->unparsed_sel = strdup(buffer);
        if (!handle_sel_time(curr_log, demeter_conf, job_info->start_time))
            continue;
        if (!handle_sel_type(curr_log))
            continue;
        if (!handle_sel_msg(curr_log))
            continue;
        if (!handle_sel_assert(curr_log))
            continue;
        sel_list = add_to_list(sel_list, init_parsed_sel());
    }
    fclose(log_file);
    return (sel_list);
}