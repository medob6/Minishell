/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_path_name.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:07:19 by salahian          #+#    #+#             */
/*   Updated: 2025/04/24 10:45:41 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     search_wildcard(char *tmp)
{
    int     i;

    i = 0;
    while (tmp[i])
    {
        if (tmp[i] == '*')
            return (1);
        i++;
    }
    return (0);
}

int     check_str(char *str)
{
    int     i;

    i = 0;
    while (str[i])
    {
        if (str[i] != '*')
            return (0);
        i++;
    }
    return (1);
}

int match_pattern(const char *pattern, const char *name)
{
    if (!pattern || !name)
        return (0);
    while (*pattern)
    {
        if (*pattern == '*')
        {
            while (*pattern == '*')
                pattern++;
            if (!*pattern)
                return (1);

            while (*name)
            {
                if (match_pattern(pattern, name))
                    return (1);
                name++;
            }
            return (0);
        }
        else if (*name && *pattern == *name)
        {
            pattern++;
            name++;
        }
        else
            return (0);
    }
    return (*name == '\0');
}

char    *expand_wild(char *path, char *get)
{
    struct dirent   *line;
    DIR     *dir;
    int     i;
    char    *new_str;
    char    *tmp;

    new_str = NULL;
    i = 0;
    dir = opendir(".");
    if (!dir)
        return (NULL);
    line = readdir(dir);
    printf("{%s}   [%s]\n", path, get);
    tmp = ft_strjoin(path, line->d_name);
    if (access(tmp, F_OK))
        return (path);
    //i need to use function access to check for expansion $ and the same for the return of new_str every line i got i need to check after joined by access every file
    while (line)
    {
        tmp = ft_strjoin(path, line->d_name);
        if (!get || check_str(get))
            new_str = ft_strjoin(new_str, ft_strjoin(line->d_name, " "));
        else if (!access(tmp, F_OK) && get && match_pattern(get, line->d_name))
            new_str = ft_strjoin(new_str, ft_strjoin(tmp, " "));
        line = readdir(dir);
    }
    closedir(dir);
    return (new_str);
}

void expand_wildcard(t_array *child, int i)
{
    char *str = child->items[i];
    char *path = NULL;
    char *pattern = NULL;
    char *slash = ft_strchr(str, '/');
    char *wild = ft_strchr(str, '*');
    char *new_str;

    if (wild && (!slash || slash > wild)) {
        pattern = str;
    } else {
        int last_slash = -1;
        for (int j = 0; str[j]; j++) {
            if (str[j] == '/')
                last_slash = j;
            if (str[j] == '*')
                break;
        }

        if (last_slash != -1) {
            path = ft_substr(str, 0, last_slash + 1);
            pattern = &str[last_slash + 1];
        } else {
            path = ".";
            pattern = str;
        }
    }

    new_str = expand_wild(path, pattern);
    if (new_str)
        child->items[i] = new_str;
}


void    expand_path_name(t_ast_node *node)
{
    char    *tmp;
    size_t     i;

    i = 0;
    while (i < node->children->length)
    {
        tmp = node->children->items[i];
        if (tmp && search_wildcard(tmp))
            expand_wildcard(node->children, i);
        i++;
    }
}
