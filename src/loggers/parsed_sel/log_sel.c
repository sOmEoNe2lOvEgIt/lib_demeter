// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such sel, much logging!
//___________________________________________________________________________________________________________________________________________

#include <string.h>
#include "demeter.h"

void log_parsed_sel(linked_list_t *gathered_sel)
{
	if (gathered_sel == NULL) {
		printf("no logs gathered");
		return;
	}
	while (gathered_sel->next != NULL && is_log_empty(((parsed_sel_t *)gathered_sel->data)->unparsed_sel))
		gathered_sel = gathered_sel->next;
	while (gathered_sel != NULL) {
    	if (gathered_sel != NULL && !is_log_empty(((parsed_sel_t *)gathered_sel->data)->unparsed_sel)) {
			printf(((parsed_sel_t *)gathered_sel->data)->unparsed_sel);
            printf("%s\n", ((parsed_sel_t *)gathered_sel->data)->sel_time_str);
            printf("%s\n", ((parsed_sel_t *)gathered_sel->data)->sel_msg_type);
            printf("%s\n", ((parsed_sel_t *)gathered_sel->data)->sel_msg);
            printf(((parsed_sel_t *)gathered_sel->data)->asserted? "Asserted\n" : "Deasserted\n");
            printf("\n");
		}
		gathered_sel = gathered_sel->next;
	}
}
