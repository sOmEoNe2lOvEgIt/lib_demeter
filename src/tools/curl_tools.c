
// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such send, much curl!
//___________________________________________________________________________________________________________________________________________

#define _GNU_SOURCE
#include <stdio.h>
#include <curl/curl.h>
#include <src/common/macros.h>
#include "demeter.h"

// CURL FUNCTION
//___________________________________________________________________________________________________________________________________________

static void curl_write_function(void *ptr, size_t size, size_t nmemb, void *stream){
    if (ptr)
        printf("demeter curl: %s", (char *)ptr);
}

static void curl_dont_write_function(void *ptr, size_t size, size_t nmemb, void *stream){
    return;
}

bool send_json(demeter_conf_t *demeter_conf, char *json_log, job_id_info_t *job_info)
{
    CURL *curl;
    CURLcode res;
    char *base_url = demeter_conf->demeter_comp_loc;
    struct curl_slist *list = NULL;

    if (!json_log)
        return (true);
    list = curl_slist_append(list, "Content-Type: application/json");
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, base_url);
        if (demeter_conf->demeter_comp_proxy) {
            curl_easy_setopt(curl, CURLOPT_PROXY, demeter_conf->demeter_comp_proxy);
        }
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_log);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)(sizeof(char) * strlen(json_log)));
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        if (demeter_conf->demeter_comp_usr && demeter_conf->demeter_comp_psswd) {
            curl_easy_setopt(curl, CURLOPT_USERNAME, demeter_conf->demeter_comp_usr);
            curl_easy_setopt(curl, CURLOPT_PASSWORD, demeter_conf->demeter_comp_psswd);
        }
        if (demeter_conf->verbose_lv >= 2)
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_function);
        else
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_dont_write_function);
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