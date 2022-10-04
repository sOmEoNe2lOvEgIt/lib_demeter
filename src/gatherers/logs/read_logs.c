// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such logs, many buffer!
//___________________________________________________________________________________________________________________________________________

#include <stdlib.h>
#include <sys/klog.h>
#include <string.h>

char *read_sys_logs(void)
{
    static char *log_buffer;
    int log_size = klogctl(10, NULL, 0);

    if (log_size <= 0)
        return (NULL);
    if ((log_buffer = malloc(sizeof(char) * log_size + 1)) == NULL)
        return (NULL);
    if (klogctl(3, log_buffer, log_size) < 0)
        return (NULL);
    log_buffer[log_size + 1] = '\0';
    return (log_buffer);
}