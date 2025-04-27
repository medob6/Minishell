/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_path_name.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:07:19 by salahian          #+#    #+#             */
/*   Updated: 2025/04/27 14:51:54 by salahian         ###   ########.fr       */
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
            //if (*pattern == '\0')
            //    return 1;
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
char    *check_string_get(char *get)
{
    int     i;
    char    *tmp;
    char    c;
    
    i = 0;
    tmp = NULL;
    while (get[i])
    {
        if (get[i] == '"' || get[i] == '\'')
        {
            c = get[i];
            i++;
            while (get[i] && get[i] != c)
            {
                tmp = append_char(tmp, get[i]);
                i++;
            }
            if (get[i] == c)
                i++;
        }
        else
            tmp = append_char(tmp, get[i]);
        if (get[i])
            i++;
    }
    return (tmp);
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
    if (path)
        dir = opendir(path);
    else
        dir = opendir(".");
    if (!dir)
        return (NULL);
    line = readdir(dir);
    tmp = ft_strjoin(path, line->d_name);
    if (check_string_get(get))
        get = check_string_get(get);
    while (line)
    {
        help_expand_wild(line, path, get, &new_str);
        line = readdir(dir);
    }
    closedir(dir);
    return (new_str);
}

// void expand_wildcard(t_array *child, int i, int flag)
// {
//     char *str;
//     char *path;
//     char *pattern;
//     char *slash;
//     char *wild;
//     char *new_str;
//     int last_slash;
//     int j;
    
//     if (flag)
// 		str = child->items[i];
// 	else
// 		str = ((t_token *)child->items[i])->value;
//     slash = ft_strchr(str, '/');
//     wild = ft_strchr(str, '*');
//     path = NULL;
//     pattern = NULL;
//     if (wild && (!slash || slash > wild))
//         pattern = str;
//     else
//     {
//         last_slash = -1;
//         j = 0;
//         while (str[j]) 
//         {
//             if (str[j] == '/')
//                 last_slash = j;
//             if (str[j] == '*')
//                 break;
//             j++;
//         }
//         if (last_slash != -1)
//         {
//             path = ft_substr(str, 0, last_slash + 1);
//             pattern = &str[last_slash + 1];
//         }
//         else
//             pattern = str;
//     }
//     new_str = expand_wild(path, pattern);
//     if (new_str)
//     {
//         if (flag)
//             child->items[i] = new_str;
//         else
//             ((t_token *)child->items[i])->value = new_str;
//     }
// }

int check_pattern(char *s)
{
	int     i;
    char    c;

    if (!s)
        return (0);
    i = 0;
	while (s[i])
    {
        if (s[i] == '"' || s[i] == '\'')
        {
            c = s[i];
            i++;
            while (s[i] && s[i] != c)
                i++;
            if (s[i] == c)
                i++;
        }
        if (s[i] == '*')
            return (0);
        if (s[i])
		    i++;
    }
	return (1);
}

char    *get_string_from_child(t_array *child, int i, int flag)
{
    if (flag)
        return (child->items[i]);
    return (((t_token *)child->items[i])->value);
}

int find_last_slash_before_star(char *str)
{
    int j;
    int last_slash;

    j = 0;
    last_slash = -1;
    while (str[j])
    {
        if (str[j] == '/')
            last_slash = j;
        if (str[j] == '*')
            break;
        j++;
    }
    return (last_slash);
}

void get_pattern_and_path(char *str, char **path, char **pattern)
{
    char    *slash;
    char    *wild;
    int     last_slash;

    slash = ft_strchr(str, '/');
    wild = ft_strchr(str, '*');
    if (wild && (!slash || slash > wild))
    {
        *pattern = str;
        *path = NULL;
    }
    else
    {
        last_slash = find_last_slash_before_star(str);
        if (last_slash != -1)
        {
            *path = ft_substr(str, 0, last_slash + 1);
            *pattern = &str[last_slash + 1];
        }
        else
            *pattern = str;
    }
}
void set_expanded_value(t_array *child, int i, int flag, char *new_str)
{
    int len;

    len = ft_strlen(new_str);
    while (len > 0 && new_str[len - 1] == ' ')
        len--;
    new_str[len] = '\0';
    if (flag)
        child->items[i] = new_str;
    else
        ((t_token *)child->items[i])->value = new_str;
}

void expand_wildcard(t_array *child, int i, int flag)
{
    char    *str;
    char    *path;
    char    *pattern;
    char    *new_str;

    str = get_string_from_child(child, i, flag);
    path = NULL;
    pattern = NULL;
    get_pattern_and_path(str, &path, &pattern);
    if (check_pattern(pattern))
        return ;
    new_str = expand_wild(path, pattern);
    if (new_str)
        set_expanded_value(child, i, flag, new_str);
}


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
