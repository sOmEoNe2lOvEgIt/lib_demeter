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

linked_list_t *gather_sel(demeter_conf_t *demeter_conf, job_id_info_t *job_info, cgroup_data_t *cgroup_data)
{
    linked_list_t *sel_list = NULL;

    sel_list = gather_sel_logs(demeter_conf, job_info, sel_list);
    return (sel_list);
}