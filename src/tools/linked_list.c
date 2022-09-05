// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such link, Much list!
//___________________________________________________________________________________________________________________________________________

#include <stdio.h>
#include "demeter.h"

linked_list_t *add_to_list(linked_list_t *list, void *data)
{
    linked_list_t *new_link = (linked_list_t *)malloc(sizeof(linked_list_t));

    if (new_link == NULL)
        return (NULL);
    if (data == NULL)
        new_link->data = NULL;
    else
        new_link->data = data;
    if (list == NULL)
        new_link->next = NULL;
    else
        new_link->next = list;
    return (new_link);
}
