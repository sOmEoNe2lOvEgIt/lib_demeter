// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such remove, no newline!
//___________________________________________________________________________________________________________________________________________

void remove_newline(char *str)
{
    int i = 0;

    for (; str[i] != '\0'; i++) {
        if (str[i] == '\n')
            str[i] = '\0';
    }
}