/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_path_name.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:07:19 by salahian          #+#    #+#             */
/*   Updated: 2025/04/23 17:57:00 by salahian         ###   ########.fr       */
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

char    *expand_wild(char *path, char *get)
{
    struct dirent   *line;
    DIR     *dir;
    int     i;
    char    *new_str;

    new_str = NULL;
    i = 0;
    //printf("%s\n", path);
    if (path)
        dir = opendir(path);
    else
        dir = opendir(".");
    if (!dir)
        return (NULL);
    line = readdir(dir);
    //printf("%s\n", line->d_name);
    //i need to use function access to check for expansion $ and the same for the return of new_str every line i got i need to check after joined by access every file
    while (line)
    {
        if (!get || check_str(get))
            new_str = ft_strjoin(new_str, ft_strjoin(line->d_name, " "));
        else if (get && ft_strlen(line->d_name) >= ft_strlen(get) &&
        ft_strncmp(&line->d_name[ft_strlen(line->d_name) - ft_strlen(get)], get, ft_strlen(get)) == 0)
            new_str = ft_strjoin(new_str, ft_strjoin(path, ft_strjoin(line->d_name, " ")));
        line = readdir(dir);
    }
    closedir(dir);
    return (new_str);
}

void    expand_wildcard(t_array *child, int i)
{
    char    *str;
    char    *new_str;
    char    *old_str;
    char    *tmp;
    int     j;

    str = child->items[i];
    new_str = NULL;
    old_str = NULL;
    tmp = NULL;
    j = 0;
    while (str[j])
    {
        if (str[j] != '*')
        {
            old_str = append_char(old_str, str[j]);
            j++;
        }
        if (str[j] == '*')
        {
            j++;
            while (str[j])
            {
                tmp = append_char(tmp, str[j]);
                j++;
            }
            new_str = expand_wild(old_str, tmp);
        }
    }
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
