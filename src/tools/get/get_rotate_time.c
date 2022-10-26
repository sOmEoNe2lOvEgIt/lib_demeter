// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such time, nice rotate!
//___________________________________________________________________________________________________________________________________________

#include <time.h>
#include <string.h>
#include <stdlib.h>

time_t get_rotate_time(char *file_name)
{
    time_t rotate_time = 0;
    char *rotate_time_str = NULL;
    struct tm *rotate_time_tm = NULL;

    if (!file_name)
        return (0);
    rotate_time_str = strstr(file_name, "log-");
    if (!rotate_time_str)
        return (0);
    rotate_time_str += 4;
    rotate_time_tm = malloc(sizeof(struct tm));
    if (!rotate_time_tm)
        return (0);
    rotate_time_tm->tm_year = atoi(strndup(rotate_time_str, 4)) - 1900;
    rotate_time_tm->tm_mon = atoi(strndup(rotate_time_str + 4, 2)) - 1;
    rotate_time_tm->tm_mday = atoi(strndup(rotate_time_str + 6, 2)) + 1;
    rotate_time_tm->tm_hour = 0;
    rotate_time_tm->tm_min = 0;
    rotate_time_tm->tm_sec = 0;
    rotate_time_tm->tm_isdst = -1;
    rotate_time = mktime(rotate_time_tm);
    free(rotate_time_tm);
    return (rotate_time);
}