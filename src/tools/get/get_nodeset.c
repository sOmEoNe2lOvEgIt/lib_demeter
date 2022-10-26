// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such nodeset, many nodes!
//___________________________________________________________________________________________________________________________________________

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *get_nodename(char *nodeset)
{
    char nodename[253]; //253 is the max len for a host name.
    int i = 0;

    if (!nodeset)
        return (NULL);
    memset(nodename, 0, 253);
    for (i = 0; nodeset[i] != '\0' && nodeset[i] != '[' &&
    (nodeset[i] < '0' || nodeset[i] > '9'); i++) {
        if (nodeset[i] == ',' || nodeset[i] == ']' || nodeset[i] == '\n')
            return (NULL);
        nodename[i] = nodeset[i];
        nodename[i + 1] = '\0';
    }
    if (nodename[0] == '\0')
        return (NULL);
    return (strdup(nodename));
}

static bool check_order(char *nodeset)
{
    int i = 0, last_val = -1;

    if (!nodeset || nodeset[i] == '\0')
        return (false);
    for (i = 0; nodeset[i] != '\0' && nodeset[i] != '['; i++);
    if (nodeset[i] == '\0')
        return (false);
    i++;
    last_val = atoi(&nodeset[i]);
    for (; nodeset[i] != '\0' && nodeset[i] != ']'; i++) {
        if (nodeset[i] == '[')
            return (false);
        if (nodeset[i] == ',' || nodeset[i] == '-') {
            if (last_val > atoi(&nodeset[i + 1]))
                return (false);
            last_val = atoi(&nodeset[i + 1]);
        }
    }
    if (nodeset[i] == '\0')
        return (false);
    return (true);
}

char *get_nodecount(char *nodeset)
{
    char *nodecount;
    int i = 0, j = 0;

    if (!nodeset)
        return (NULL);
    if (!strstr(nodeset, "[") && strstr(nodeset, "]"))
        return (NULL);
    nodecount = malloc(sizeof(char) * strlen(nodeset));
    if (!nodecount)
        return (NULL);
    if (!strstr(nodeset, "[") && !strstr(nodeset, "]")) {
        for (i = 0; nodeset[i] != '\0' && nodeset[i] != '\n' &&
        (nodeset[i] < '0' || nodeset[i] > '9'); i++);
        for (;nodeset[i] >= '0' && nodeset[i] <= '9'; i++, j++)
            nodecount[j] = nodeset[i];
        nodecount[j] = '\0';
        if(nodecount[0] == '\0') {
            free(nodecount);
            return (strdup("-1"));
        }
        if ((nodeset[i] != '\0' && nodeset[i] != '\n')){
            free(nodecount);
            return (NULL);
        }
        return(nodecount);
    }
    for (i = 0; nodeset[i] != '\0' && nodeset[i] != '['; i++);
    if (nodeset[i] == '\0'){
        free(nodecount);
        return (NULL);
    }
    if (!check_order(nodeset)) {
        free(nodecount);
        return (NULL);
    }
    for (i++, j = 0; nodeset[i] != '\0' && nodeset[i] != ']'; i++, j++)
        nodecount[j] = nodeset[i];
    nodecount[j] = '\0';
    if (nodecount[0] == '\0') {
        free(nodecount);
        return (NULL);
    }
    return (nodecount);
}

int get_next_node(char *clean_nodecount, int curr_node)
{
    int i = 0, last_val = -1;

    if (!clean_nodecount)
        return (-84);
    last_val = atoi(&clean_nodecount[i]);
    for (i = 0; clean_nodecount[i] != '\0' && last_val <= curr_node; i++) {
        if (clean_nodecount[i] == ',' || clean_nodecount[i] == '-') {
            last_val = atoi(&clean_nodecount[i + 1]);
        }
    }
    if (clean_nodecount[i] == '\0' &&
    last_val <= curr_node)
        return (-1);
    return (last_val);
}

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