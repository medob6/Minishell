/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_path_name.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:07:19 by salahian          #+#    #+#             */
/*   Updated: 2025/04/24 18:12:43 by salahian         ###   ########.fr       */
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
            if (*pattern == '\0')
                return 1;
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
char    *handle_dir(struct dirent *dir, char *get, char *path, char *new_str)
{
    int     i;
    int     header;
    char    *tmp;

    header = 0;
    if (get[0] == '.')
        header = 1;
    i = header;
    while (get[i] == '*')
        i++;
    tmp = ft_strjoin(dir->d_name, &get[i]);
    if (header)
    {
        if (get[i] == '/' && get[i + 1] == '\0' && dir->d_name[0] == '.')
            return (ft_strjoin(new_str, ft_strjoin(ft_strjoin(path, tmp), " ")));
    }
    else
    {
        if (get[i] == '/' && get[i + 1] == '\0' && dir->d_name[0] != '.')
            return (ft_strjoin(new_str, ft_strjoin(ft_strjoin(path, tmp), " ")));
    }
    return (NULL);
}

void help_expand_wild(struct dirent *dir, char *path, char *get, char **new_str)
{
    char *tmp;

    tmp = ft_strjoin(path, dir->d_name);
    if ((!get || check_str(get)) && dir->d_name[0] != '.')
        *new_str = ft_strjoin(*new_str, ft_strjoin(tmp, " "));
    else if (get && match_pattern(get, dir->d_name) && dir->d_name[0] != '.')
    {
        *new_str = ft_strjoin(*new_str, ft_strjoin(tmp, " "));
    }
    else if (get && dir->d_type == 4)
    {
        tmp = handle_dir(dir, get, path, *new_str);
        if (tmp)
            *new_str = tmp;
    }
}
//!access(tmp, F_OK) && 
char    *expand_wild(char *path, char *get)
{
    struct dirent   *line;
    DIR     *dir;
    int     i;
    char    *new_str;
    char    *tmp;

    new_str = NULL;
    i = 0;
    if (path)
        dir = opendir(path);
    else
        dir = opendir(".");
    if (!dir)
        return (NULL);
    line = readdir(dir);
    tmp = ft_strjoin(path, line->d_name);
    if (access(tmp, F_OK))
        return (path);
    while (line)
    {
        help_expand_wild(line, path, get, &new_str);
        line = readdir(dir);
    }
    closedir(dir);
    return (new_str);
}

void expand_wildcard(t_array *child, int i, int flag)
{
    char *str = child->items[i];
    char *path = NULL;
    char *pattern = NULL;
    char *slash = ft_strchr(str, '/');
    char *wild = ft_strchr(str, '*');
    char *new_str;

    if (wild && (!slash || slash > wild))
        pattern = str;
    else
    {
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
    {
        if (flag)
            child->items[i] = new_str;
        else
            ((t_token *)child->items[i])->value = new_str;
    }
}

// void    expand_wildcard(t_array *child, int i)
// {
//     char    *str;
//     char    *new_str;
//     char    *old_str;
//     char    *tmp;
//     int     j;
//     int     flag = 1;

//     str = (char *)child->items[i];
//     new_str = NULL;
//     old_str = NULL;
//     tmp = NULL;
//     j = 0;
//     printf("[%s]\n", str);
//     while (str[j])
//     {
//         if (flag)
//         {
//             tmp = append_char(tmp, str[j]);
//             j++;
//         }
//         if (str[j] != '*')
//         {
//             if (str[j] == '\'')
//                 flag = 1;
//             old_str = append_char(old_str, str[j]);
//             j++;
//         }
//         if (str[j] == '*')
//         {
//             while (str[j])
//             {
//                 tmp = append_char(tmp, str[j]);
//                 j++;
//             }
//             new_str = expand_wild(old_str, tmp);
//         }
//     }
//     if (new_str)
//         child->items[i] = new_str;
// }


void    expand_path_name_cmd(t_ast_node *node)
{
    char    *tmp;
    size_t     i;

    i = 0;
    if (!node->children)
        return ;
    while (i < node->children->length)
    {
        tmp = node->children->items[i];
        //printf("[%s]\n", tmp);
        if (tmp && search_wildcard(tmp))
            expand_wildcard(node->children, i, 1);
        i++;
    }
}

void    expand_path_name_red(t_ast_node *node)
{
    char    *tmp;
    size_t     i;

    i = 0;
    if (!node->redirect_list)
		return ;
	while (i < node->redirect_list->length)
	{
		if (((t_token *)node->redirect_list->items[i])->type == TOKEN_HEREDOC)
		{
			i++;
			continue;
		}
		tmp = ((t_token *)node->redirect_list->items[i])->value;
		if (tmp && search_wildcard(tmp))
            expand_wildcard(node->redirect_list, i, 0);
		i++;
	}
}
