// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, much logging!
//___________________________________________________________________________________________________________________________________________

#include <string.h>
#include "demeter.h"

void log_parsed_logs(linked_list_t *gathered_logs, demeter_conf_t *demeter_conf)
{
	if (gathered_logs == NULL) {
		write_log_to_file(demeter_conf, "no logs gathered", DEBUG, 3);
		return;
	}
	while (gathered_logs->next != NULL && is_log_empty(((parsed_log_t *)gathered_logs->data)->unparsed_log))
		gathered_logs = gathered_logs->next;
	while (gathered_logs != NULL) {
    	if (gathered_logs != NULL && !is_log_empty(((parsed_log_t *)gathered_logs->data)->unparsed_log)) {
			// write_log_to_file(demeter_conf, "logs gathered", DEBUG, 3);
			write_log_to_file(demeter_conf, ((parsed_log_t *)gathered_logs->data)->unparsed_log, DEBUG, 3);
			// write_log_to_file(demeter_conf, ((parsed_log_t *)gathered_logs->data)->log_time_str, DEBUG, 3);
		} else
			write_log_to_file(demeter_conf, "no worth logs gathered", DEBUG, 3);
		gathered_logs = gathered_logs->next;
	}
}