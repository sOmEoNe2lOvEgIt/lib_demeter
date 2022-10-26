// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, much logging!
//___________________________________________________________________________________________________________________________________________

#include <string.h>
#include "demeter.h"

void log_parsed_logs(linked_list_t *gathered_logs, demeter_conf_t *demeter_conf)
{
    if (!gathered_logs) {
        write_log_to_file(demeter_conf, "no logs gathered", INFO, 0);
        return;
    }
    while (gathered_logs->next && is_log_empty(((parsed_log_t *)gathered_logs->data)->unparsed_log))
        gathered_logs = gathered_logs->next;
    while (gathered_logs) {
        if (gathered_logs && !is_log_empty(((parsed_log_t *)gathered_logs->data)->unparsed_log)) {
            write_log_to_file(demeter_conf, ((parsed_log_t *)gathered_logs->data)->unparsed_log, INFO, 0);
        } else
            write_log_to_file(demeter_conf, "no worth logs gathered", DEBUG, 2);
        gathered_logs = gathered_logs->next;
    }
}