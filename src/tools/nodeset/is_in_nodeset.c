// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such nodeset, many nodes!
//___________________________________________________________________________________________________________________________________________

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "demeter.h"

static bool is_in_nodecount(char *clean_nodecount, int node)
{
    int i = 0, last_val = -1, max_val = 0;

    if (!clean_nodecount)
        return (false);
    last_val = atoi(&clean_nodecount[i]);
    for (i = 0; clean_nodecount[i] != '\0' && last_val < node; i++) {
        if (clean_nodecount[i] == '-') {
            max_val = atoi(&clean_nodecount[i + 1]);
            if (node <= max_val && node >= last_val)
                return (true);
            last_val = max_val;
        }
        if (clean_nodecount[i] == ',') {
            last_val = atoi(&clean_nodecount[i + 1]);
        }
    }
    if (last_val == node)
            return (true);
    return (false);
}

bool is_in_nodeset(char *curr_node, char *nodeset)
{
    char *nodeset_nodename = get_nodename(nodeset),
    *curr_node_nodename = get_nodename(curr_node);
    char *nodeset_nodecount = get_nodecount(nodeset),
    *curr_node_nodecount = get_nodecount(curr_node);
    bool ret = true;

    if (!nodeset_nodename || !curr_node_nodename ||
    !nodeset_nodecount || !curr_node_nodecount) {
        if (nodeset_nodename)
            free(nodeset_nodename);
        if (curr_node_nodename)
            free(curr_node_nodename);
        if (nodeset_nodecount)
            free(nodeset_nodecount);
        if (curr_node_nodecount)
            free(curr_node_nodecount);
        return (false);
    }
    if (strcmp(nodeset_nodename, curr_node_nodename) != 0)
        ret = false;
    if (!is_in_nodecount(nodeset_nodecount, atoi(curr_node_nodecount)))
        ret = false;
    free(nodeset_nodename);
    free(curr_node_nodename);
    free(nodeset_nodecount);
    free(curr_node_nodecount);
    return (ret);
}