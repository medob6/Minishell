/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_path_name.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:07:19 by salahian          #+#    #+#             */
/*   Updated: 2025/05/07 15:30:50 by salahian         ###   ########.fr       */
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
        //if (get[i] == '/' && get[i + 1] == '\0' &&
        if (dir->d_name[0] == '.')
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
            tmp = append_char(tmp, get[i++]);
        //if (get[i])
            //i++;
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


//int check_pattern(char *s)
//{
//	int     i;
//    char    c;

//    if (!s)
//        return (0);
//    i = 0;
//	while (s[i])
//    {
//        if (s[i] == '"' || s[i] == '\'')
//        {
//            c = s[i];
//            i++;
//            while (s[i] && s[i] != c)
//                i++;
//            if (s[i] == c)
//                i++;
//        }
//        if (s[i] == '*')
//            return (0);
//        if (s[i])
//		    i++;
//    }
//	return (1);
//}

int	check_pattern(char *s)
{
	int		i;
	char	quote;

	if (!s)
		return (0);
	i = 0;
	quote = 0;
	while (s[i])
	{
		if (!quote && (s[i] == '"' || s[i] == '\''))
			quote = s[i++];
		while (quote && s[i] && s[i] != quote)
			i++;
		if (quote && s[i] == quote)
		{
			quote = 0;
			i++;
			continue;
		}
		if (!quote && s[i] == '*')
			return (0);
		if (s[i])
			i++;
	}
	return (1);
}


char    **get_string_from_child(t_array *child, int i)
{
    return (((t_str *)child->items[i])->value);
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
void set_expanded_value(t_array *child, int i, char **new_str)
{
    ((t_str *)child->items[i])->value = new_str;
}


char *expand_wildcard(char *str)
{
    char    *path;
    char    *pattern;
    char    *new_str;

    path = NULL;
    pattern = NULL;
    get_pattern_and_path(str, &path, &pattern);
    if (check_pattern(pattern))
        return (NULL);
    new_str = expand_wild(path, pattern);
    //if (!new_str)
    //    return (str);
    return (new_str);
}

char    **append_to_arr(char **old, char *str, int flag)
{
    int     i = 0;
    int     j = 0;
    int     count = 0;
    char    **new;
    char    **words = NULL;

    while (old && old[i])
        i++;
    if (flag)
    {
        words = ft_split(str, ' ');
        while (words && words[count])
            count++;
    }
    else
        count = 1;
    new = ft_calloc(sizeof(char *), i + count + 1);
    if (!new)
        return (NULL);
    j = 0;
    while (old && old[j])
    {
        new[j] = ft_strdup(old[j]);
        j++;
    }
    if (flag)
    {
        int k = 0;
        while (words && words[k])
        {
            new[j++] = ft_strdup(words[k]);
            k++;
        }
    }
    else
        new[j++] = ft_strdup(str);

    new[j] = NULL;
    return (new);
}


void    call_expand_wildcard(t_array *child, int i)
{
    char    **str;
    char    **new;
    int     j;

    str = get_string_from_child(child, i);
    j = 0;
    new = NULL;
    while (str && str[j])
        j++;
    new = ft_calloc(sizeof(char *), j + 1);
    j = 0;
    while (str && str[j])
    {
        if (search_wildcard(str[j]))
        {
            if (expand_wildcard(str[j]))
                new = append_to_arr(new, expand_wildcard(str[j]), 1);
            else
                new = append_to_arr(new, str[j], 0);
        }
        else
            new = append_to_arr(new, str[j], 0);
        j++;
    }
    set_expanded_value(child, i, new);
}

//void	printl(char **s)
//{
//	int		i;

//	i = 0;
//	while (s[i])
//	{
//		printf("here[%s]\n", s[i]);
//		i++;
//	}
//}

void    expand_path_name_cmd(t_expansion *expand)
{
    char    **tmp;
    size_t     i;

    i = 0;
    if (!expand->node->children)
        return ;
    while (i < expand->node->children->length)
    {
        tmp = ((t_str *)expand->node->children->items[i])->value;
        if (tmp && *tmp)
        {
            call_expand_wildcard(expand->node->children, i);
        }
        i++;
    }
}

void    expand_path_name_red(t_expansion *expand)
{
    char    **tmp;
    size_t     i;

    i = 0;
    if (!expand->node->redirect_list)
		return ;
	while (i < expand->node->redirect_list->length)
	{
		if (((t_str *)expand->node->redirect_list->items[i])->type == TOKEN_HEREDOC)
		{
			i++;
			continue;
		}
		tmp = ((t_str *)expand->node->redirect_list->items[i])->value;
		if (tmp && *tmp)
            call_expand_wildcard(expand->node->redirect_list, i);
		i++;
	}
}

// export a='*' && export b='"' && echo $b$a$b 
// echo "'$err'" --> '$err' but we need this ''
// expo''rt
