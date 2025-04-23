#include "minishell.h"

char    *applicate_field_split(char *str)
{
    char    *tmp;
    char    *new_str;
    int     j;
    int     start;

    j = 0;
    new_str = NULL;
    start = 0;
    tmp = NULL;
    while (str && str[j])
    {
        while (str[j] && (str[j] == ' ' || str[j] == '\t' || str[j] == '\n'))
            j++;
        if (!str[j])
            break;
        start = j;
        while (str[j] && str[j] != ' ' && str[j] != '\t' && str[j] != '\n')
            j++;
        tmp = ft_substr(str, start, j - start);
        if (new_str)
            new_str = ft_strjoin(ft_strjoin(new_str, " "), tmp);
        else
            new_str = ft_strdup(tmp);
    }
    return (new_str);
}

char *applicate(char *str)
{
    char *tmp = NULL;
    char *new_str = NULL;
    int j = 0;
    int start;

    while (str && str[j])
    {
        // Skip IFS (space, tab, newline)
        while (str[j] == ' ' || str[j] == '\t' || str[j] == '\n')
            j++;

        if (!str[j])
            break;

        start = j;

        // Find end of word
        while (str[j] && str[j] != ' ' && str[j] != '\t' && str[j] != '\n')
            j++;

        tmp = ft_substr(str, start, j - start);

        if (new_str)
        {
            char *joined = ft_strjoin(new_str, " ");
            free(new_str);
            new_str = ft_strjoin(joined, tmp);
            free(joined);
        }
        else
        {
            new_str = ft_strdup(tmp);
        }

        free(tmp);
    }

    return new_str;
}


int main(int ac , char **av)
{
    printf("[%s]\n", applicate_field_split(av[1]));
    printf("[%s]\n", applicate(av[1]));
}