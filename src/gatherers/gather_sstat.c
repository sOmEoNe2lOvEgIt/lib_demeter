// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such stat, many info!
//___________________________________________________________________________________________________________________________________________

#define _GNU_SOURCE
#include <stdio.h>
#include <syslog.h>
#include "slurm/slurm.h"
#include "src/common/slurm_jobacct_gather.h"
#include "demeter.h"

int sstat_pull(uint job_id, uint step_id, demeter_conf_t *demeter_conf)
{
	return (0);
    int rc = 0;
	job_step_stat_t *step_stat = NULL;
	job_step_stat_response_msg_t *resp = NULL;
	slurm_step_id_t step_id_struct = {job_id, NO_VAL, step_id};
	char rcstr[29], teststr[10000];
	ListIterator itr;

	// First, we need to get the step stat.
    if ((rc = slurm_job_step_stat(&step_id_struct, NULL, SLURM_PROTOCOL_VERSION, &resp)) != SLURM_SUCCESS) { // even when no err, gets bad values.
		sprintf(rcstr, "return code is :%d", rc);
		write_log_to_file(demeter_conf, rcstr, INFO, 0);
		if (rc == ESLURM_INVALID_JOB_ID || rc == ESLURM_ALREADY_DONE)
			write_log_to_file(demeter_conf, "step has already completed", INFO, 0);
		else
			write_log_to_file(demeter_conf, "problem getting step_layout for step", INFO, 0);
		slurm_job_step_pids_response_msg_free(resp);
		return rc;
	}
	// Then, we need to iterate over the list of job_step_stat_t.
	itr = slurm_list_iterator_create(resp->stats_list);
	while ((step_stat = slurm_list_next(itr))) {
		if (step_stat && step_stat->jobacct) {
			sprintf (teststr, "act_cpufreq: %u, user_cpu_usec: %u", step_stat->jobacct->act_cpufreq, step_stat->jobacct->user_cpu_usec); // Sends me stupuid values, but it's not the problem.
			write_log_to_file(demeter_conf, teststr, INFO, 0);
		}
		if (!step_stat)
			write_log_to_file(demeter_conf, "step_stat is NULL", INFO, 0);
		if (!step_stat->jobacct)
			write_log_to_file(demeter_conf, "step_stat->jobacct is NULL", INFO, 0);
	}
	slurm_list_iterator_destroy(itr);
	slurm_job_step_pids_response_msg_free(resp);
	return rc;
}

// {
// 	ListIterator itr = NULL;
// 	slurm_step_id_t step_id = {
// 		.job_id = 0,
// 		.step_id = NO_VAL,
// 		.step_het_comp = NO_VAL,
// 	};
// 	slurm_selected_step_t *selected_step = NULL;

// 	slurm_conf_init(NULL);
// 	print_fields_list = list_create(NULL);
// 	print_fields_itr = list_iterator_create(print_fields_list);

// 	parse_command_line(argc, argv);
// 	if (!params.opt_job_list || !list_count(params.opt_job_list)) {
// 		error("You didn't give me any jobs to stat.");
// 		return 1;
// 	}

// 	print_fields_header(print_fields_list);
// 	itr = list_iterator_create(params.opt_job_list);
// 	while ((selected_step = list_next(itr))) {
// 		job_step_info_response_msg_t *step_info = NULL;

// 		memcpy(&step_id, &selected_step->step_id, sizeof(step_id));

// 		if (slurm_get_job_steps(0, step_id.job_id, step_id.step_id,
// 					&step_info, SHOW_ALL)) {
// 			error("couldn't get steps for job %u",
// 			      selected_step->step_id.job_id);
// 			continue;
// 		} else if (!step_info->job_step_count) {
// 			if (step_id.step_id == NO_VAL)
// 				error("No steps running for job %u",
// 				      selected_step->step_id.job_id);
// 			else
// 				error("%ps not found running.",
// 				      &selected_step->step_id);

// 			continue;
// 		}

// 		for (int i = 0; i < step_info->job_step_count; i++) {
// 			/* If no stepid was requested set it to the first one */
// 			if (step_id.step_id == NO_VAL) {
// 				/*
// 				 * If asking for no particular step skip the
// 				 * special steps.
// 				 */
// 				if (!params.opt_all_steps &&
// 				    (step_info->job_steps[i].step_id.step_id >
// 				     SLURM_MAX_NORMAL_STEP_ID))
// 					continue;
// 				step_id.step_id = step_info->job_steps[i].
// 					step_id.step_id;
// 			}

// 			if (!params.opt_all_steps &&
// 			    !verify_step_id(&step_info->job_steps[i].step_id,
// 					    &step_id))
// 				continue;

// 			_do_stat(&step_info->job_steps[i].step_id,
// 				 step_info->job_steps[i].nodes,
// 				 step_info->job_steps[i].cpu_freq_min,
// 				 step_info->job_steps[i].cpu_freq_max,
// 				 step_info->job_steps[i].cpu_freq_gov,
// 				 step_info->job_steps[i].start_protocol_ver);
// 		}
// 	}
// 	list_iterator_destroy(itr);

// 	xfree(params.opt_field_list);
// 	FREE_NULL_LIST(params.opt_job_list);
// 	if (print_fields_itr)
// 		list_iterator_destroy(print_fields_itr);
// 	FREE_NULL_LIST(print_fields_list);

// 	return 0;
// }
// {