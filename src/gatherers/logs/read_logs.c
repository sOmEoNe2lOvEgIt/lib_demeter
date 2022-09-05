// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, many buffer!
//___________________________________________________________________________________________________________________________________________

#include <sys/klog.h>
#include <string.h>

char *read_sys_logs(void)
{
    static char log_buffer[16384];

    if (klogctl(3, log_buffer, 16384) < 0)
        return (NULL);
    log_buffer[16384] = '\0';
    return (strdup(log_buffer));
}