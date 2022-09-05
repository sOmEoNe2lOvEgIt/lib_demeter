// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such sel, much logging!
//___________________________________________________________________________________________________________________________________________

#include <string.h>
#include "demeter.h"

void log_parsed_sel(linked_list_t *gathered_sel, demeter_conf_t *demeter_conf)
{
	if (gathered_sel == NULL) {
		write_log_to_file(demeter_conf, "no logs gathered", DEBUG, 3);
		return;
	}
	while (gathered_sel->next != NULL && is_log_empty(((parsed_sel_t *)gathered_sel->data)->unparsed_sel))
		gathered_sel = gathered_sel->next;
	while (gathered_sel != NULL) {
    	if (gathered_sel != NULL && !is_log_empty(((parsed_sel_t *)gathered_sel->data)->unparsed_sel)) {
			// write_log_to_file(demeter_conf, "logs gathered", DEBUG, 3);
			write_log_to_file(demeter_conf, ((parsed_sel_t *)gathered_sel->data)->unparsed_sel, DEBUG, 3);
			write_log_to_file(demeter_conf, ((parsed_sel_t *)gathered_sel->data)->sel_time_str, DEBUG, 3);
		} else
			write_log_to_file(demeter_conf, "no worth logs gathered", DEBUG, 3);
		gathered_sel = gathered_sel->next;
	}
}