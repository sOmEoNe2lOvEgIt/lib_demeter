// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such plugin, much data!
//___________________________________________________________________________________________________________________________________________

#include <time.h>
#include <string.h>
#include <stdio.h>

char *get_time_str(void)
{
    char time_str[80];
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    sprintf(time_str, "[%d-%02d-%02dT%02d:%02d:%02d]", (timeinfo->tm_year + 1900), (timeinfo->tm_mon + 1), timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    return (strdup(time_str));
}