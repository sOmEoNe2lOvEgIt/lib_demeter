// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such send, much elastic!
//___________________________________________________________________________________________________________________________________________

#include <curl/curl.h>
#include "demeter.h"

int send_elastic(demeter_conf_t *demeter_conf ,job_id_info_t *job_info, cgroup_data_t *cgroup_data,
linked_list_t *gathered_logs, linked_list_t *gathered_sel, perf_data_t *gathered_perf_data)
{
    CURL *curl;
    CURLcode res;
    char *url = "http://elastic:9200/slurm/_doc"; //Has te be pulled from config file

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url); 
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            write_log_to_file(demeter_conf, "curl_easy_perform() failed... We'ere screwed...", WARNING, 0);
        curl_easy_cleanup(curl);
    }
    return (0);
}