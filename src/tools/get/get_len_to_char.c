// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, much len, such char!
//___________________________________________________________________________________________________________________________________________

int get_len_to_char(char *str, char c)
{
    int i = 0;

    while (str[i] != c && str[i] != '\0')
        i++;
    return (i);
}
