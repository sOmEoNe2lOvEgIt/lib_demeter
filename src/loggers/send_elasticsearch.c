// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such send, much elastic!
//___________________________________________________________________________________________________________________________________________

#define _GNU_SOURCE
#include <stdio.h>
#include <curl/curl.h>
#include "demeter.h"
#include "src/common/macros.h"

static bool send_log(demeter_conf_t *demeter_conf, char *json_log, job_id_info_t *job_info)
{
    CURL *curl;
    CURLcode res;
    char *base_url = "http://elastic:9200/slurm_demeter/_doc"; //url has te be pulled from config file
    struct curl_slist *list = NULL;

    if (json_log == NULL)
        return (true);
    list = curl_slist_append(list, "Content-Type: application/json");
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, base_url);
	    curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_log);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)(sizeof(char) * strlen(json_log)));
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            write_log_to_file(demeter_conf, "curl_easy_perform() failed... We'ere screwed...", DEBUG, 5);
        else
            write_log_to_file(demeter_conf, "curl_easy_perform() success", DEBUG, 5);
        curl_easy_cleanup(curl);
    }
    curl_slist_free_all(list);
    if (res != CURLE_OK)
        return false;
    return true;
}

int send_elastic(demeter_conf_t *demeter_conf ,job_id_info_t *job_info, linked_list_t *cgroup_data,
linked_list_t *gathered_logs, log_counter_t *log_counter, linked_list_t *gathered_sel, perf_data_t *gathered_perf_data)
{
    char *json_log, *tmp;

    json_log = strdup("{");
    tmp = format_job_info(job_info);
    json_log = append_str(json_log, tmp);
    free(tmp);
    json_log = append_str(json_log, strdup("\"job_data\":{"));
    tmp = format_logs(gathered_logs, gathered_sel);
    json_log = append_str(json_log, tmp);
    if (tmp != NULL)
        free(tmp);
    json_log = append_str(json_log, ", ");
    tmp = format_cgroup(cgroup_data);
    json_log = append_str(json_log, tmp);
    if (tmp != NULL)
        free(tmp);
    json_log = append_str(json_log, ", ");
    tmp = format_log_counter(log_counter);
    json_log = append_str(json_log, tmp);
    if (tmp != NULL)
        free(tmp);
    json_log = append_str(json_log, ", ");
    tmp = format_perf_count(gathered_perf_data);
    json_log = append_str(json_log, tmp);
    if (tmp != NULL)
        free(tmp);
    json_log = append_str(json_log, "}}");
    if (send_log(demeter_conf, json_log, job_info) == false)
        write_log_to_file(demeter_conf, "Failed to send log to elastic", DEBUG, 2);
    // write_log_to_file(demeter_conf, json_log, INFO, 0);
    if (json_log != NULL)
        free(json_log);
    return (0);
}
