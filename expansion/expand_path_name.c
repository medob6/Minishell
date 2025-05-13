/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_path_name.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:07:19 by salahian          #+#    #+#             */
/*   Updated: 2025/05/13 09:26:20 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int find_last_slash(char *str)
{
    int j;
    int last_slash;

    j = 0;
    last_slash = -1;
    while (str[j])
    {
        if (str[j] == '/')
            last_slash = j;
        j++;
    }
    return (last_slash);
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

//int match_pattern(char *field, char *pattern, char *name)
//{
//    while (*pattern)
//    {
//        if (*pattern == '*' && (*field == '0' || *field == '1'))
//        {
//            while (*pattern == '*' && (*field == '0' || *field == '1'))
//            {
//                field++;
//                pattern++;
//            }
//            if (*pattern == '\0')
//                return (1);
//            while (*name)
//            {
//                if (match_pattern(field, pattern, name))
//                    return (1);
//                name++;
//            }
//            return (0);
//        }
//        else if (*name && *pattern == *name)
//        {
//            field++;
//            pattern++;
//            name++;
//        }
//        else
//            return (0);
//    }
//    return (*name == '\0');
//}

int skip_wildcards(char **pattern, char **field)
{
    while (**pattern == '*' && (**field == '0' || **field == '1'))
    {
        (*pattern)++;
        (*field)++;
    }
    return (**pattern == '\0');
}

int try_match_rest(char *field, char *pattern, char *name)
{
    while (*name)
    {
        if (match_pattern(field, pattern, name))
            return (1);
        name++;
    }
    return (0);
}

int match_pattern(char *field, char *pattern, char *name)
{
    while (*pattern)
    {
        if (*pattern == '*' && (*field == '0' || *field == '1'))
        {
            if (skip_wildcards(&pattern, &field))
                return (1);
            return (try_match_rest(field, pattern, name));
        }
        else if (*name && *pattern == *name)
        {
            field++;
            pattern++;
            name++;
        }
        else
            return (0);
    }
    return (*name == '\0');
}


char    *handle_dir(struct dirent *dir, char *get, char *path, char *field)
{
    int     slash;
    int     header;
    char    *tmp;
    char    *new;

    header = (get[0] == '.');
    slash = find_last_slash(get);
    if (slash != -1)
        new = ft_substr(get, 0, slash);
    else
        new = ft_strdup(get);
    tmp = ft_strdup(dir->d_name);
    if (header)
    {
        if (dir->d_name[0] == '.' && match_pattern(field, new, dir->d_name))
        {
            if (slash != -1)
                return (ft_strjoin(ft_strjoin(path, tmp), "/ "));
            return (ft_strjoin(ft_strjoin(path, tmp), " ")); 
        }
    }
    else
    {
        if (dir->d_name[0] != '.' && match_pattern(field, new, dir->d_name))
        {
            if (slash != -1)
                return (ft_strjoin(ft_strjoin(path, tmp), "/ "));
            return (ft_strjoin(ft_strjoin(path, tmp), " "));
        }
    }
    return (NULL);
}
//char *handle_dir(struct dirent *dir, char *get, char *path, char *field)
//{
//    int     i;
//    int     slash;
//    int     header;
//    char    *tmp;
//    char    *new;
//    char    *result = NULL;
//    char    *full_path;
//    char    *new_field;

//    header = (get[0] == '.');
//    i = header;
//    while (get[i] == '*')
//        i++;
//    slash = find_last_slash(&get[i]);
//    if (slash != -1)
//    {
//        new_field = ft_substr(field, 0, ft_strlen(field) - 1);
//        new = ft_substr(get, i, slash);
//    }
//    else
//    {
//        new_field = ft_strdup(field);
//        new = ft_strdup(&get[i]);
//    }
//    //printf("[%s]\n", new);
//    //printf("here////////////////////////[%d]\n", match_pattern(new_field, new, dir->d_name));
//    tmp = ft_strdup(dir->d_name);
//    if ((header && dir->d_name[0] == '.' && match_pattern(new_field, new, dir->d_name)) ||
//        (!header && dir->d_name[0] != '.' && match_pattern(new_field, new, dir->d_name)))
//    {
//        full_path = ft_strjoin(path, tmp);
//        if (slash != -1)
//            result = ft_strjoin(full_path, "/ ");
//        else
//            result = ft_strjoin(full_path, " ");
//    }
//    return (result);
//}


//void help_expand_wild(struct dirent *dir, char *path, char *get, char **new_str, char *field)
//{
//    char *tmp;

//    tmp = ft_strjoin(path, dir->d_name);
//    //if ((!get || check_str(get)) && dir->d_name[0] != '.')
//    //    *new_str = ft_strjoin(*new_str, ft_strjoin(tmp, " "));
//    if (get && match_pattern(field, get, dir->d_name) && dir->d_name[0] != '.')
//    {
//        *new_str = ft_strjoin(*new_str, ft_strjoin(tmp, " "));
//    }
//    else if (get && dir->d_type == 4)
//    {
//        tmp = handle_dir(dir, get, path, *new_str);
//        if (tmp)
//            *new_str = tmp;
//    }
//}

char *help_expand_wild(struct dirent *dir, char *path, char *get, char *field)
{
    char *tmp;
    char *result;

    result = NULL;
    tmp = ft_strjoin(path, dir->d_name);
    if (get && match_pattern(field, get, dir->d_name) && dir->d_name[0] != '.')
    {
        result = ft_strjoin(tmp, " ");
    }
    else if (get && dir->d_type == 4)
    {
        result = handle_dir(dir, get, path, field);
    }
    return (result);
}


char    *check_string_is_not_null(char *get)
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
    }
    return (tmp);
}

char    *help_check_string_get(char **field, char **new, char *get, int *i)
{
    char    c;
    char    *tmp;

    tmp = NULL;
    c = get[(*i)++];
    while (get[*i] && get[*i] != c)
    {
        *new = append_char(*new, field[0][*i]);
        tmp = append_char(tmp, get[(*i)++]);
    }
    if (get[*i] == c)
        (*i)++;
    return (tmp);
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

char    *check_string_get(char **field, char *get)
{
    int     i;
    char    *tmp;
    char    *new;
    
    i = 0;
    tmp = NULL;
    new = NULL;
    while (get[i])
    {
        if (get[i] == '"' || get[i] == '\'')
            tmp = ft_strjoin(tmp, help_check_string_get(field, &new, get, &i));
        else
        {
            new = append_char(new, field[0][i]);
            tmp = append_char(tmp, get[i++]);
        }
    }
    *field = new;
    return (tmp);
}

//char    *expand_wild(char **field, char *path, char *get)
//{
//    struct dirent   *file;
//    DIR     *dir;
//    int     i;
//    char    *new_str;
//    char    *tmp;

//    new_str = NULL;
//    i = 0;
//    if (path)
//        dir = opendir(path);
//    else
//        dir = opendir(".");
//    if (!dir)
//        return (NULL);
//    file = readdir(dir);
//    tmp = ft_strjoin(path, file->d_name);
//    if (check_string_is_not_null(get))
//        get = check_string_get(field, get);
//    while (file)
//    {
//        help_expand_wild(file, path, get, &new_str, *field);
//        file = readdir(dir);
//    }
//    closedir(dir);
//    return (new_str);
//}

char *expand_wild(char **field, char *path, char *get)
{
    struct dirent *file;
    DIR *dir;
    char *new_str;
    char *add;

    if (path)
        dir = opendir(path);
    else
        dir = opendir(".");
    if (!dir)
        return (NULL);
    new_str = NULL;
    file = readdir(dir);
    if (check_string_is_not_null(get))
        get = check_string_get(field, get);
    //printf("[%s]\n", get);
    while (file)
    {
        add = help_expand_wild(file, path, get, *field);
        if (add)
            new_str = ft_strjoin(new_str, add);
        file = readdir(dir);
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

char    *take_before_wildcard(char *field, char **org, int j, int *i)
{
    int     len = 0;
    int     k = 0;
    char    *res;

    while (k < j)
    {
        len += ft_strlen(org[k]);
        k++;
    }
    *i = len;
    res = malloc(len + 1);
    if (!res)
        return (NULL);
    ft_strlcpy(res, field, len + 1);
    return (res);
}


char *expand_wildcard(char **field, char *str)
{
    char    *path;
    char    *pattern;
    char    *new_str;

    path = NULL;
    pattern = NULL;
    get_pattern_and_path(str, &path, &pattern);
    if (check_pattern(pattern))
        return (NULL);
    new_str = expand_wild(field, path, pattern);
    //if (!new_str)
    //    return (str);
    return (new_str);
}
//char    **fill_arr(char **words, char **new, char *str, int flag)
//{
    
//}
static char     **split_new(char *str, int *count, int flag)
{
    char    **words;

    words = NULL;
    if (flag)
    {
        *count = 0;
        words = ft_split(str, ' ');
        while (words && words[*count])
            (*count)++;
    }
    else
        *count = 1;
    return (words);
}

static char    **append_old_arr(char **old, int *j, int i, int count)
{
    char    **new;

    new = ft_calloc(sizeof(char *), i + count + 1);
    while (old && old[*j])
    {
        new[*j] = ft_strdup(old[*j]);
        (*j)++;
    }
    return (new);
}

char    **append_to_arr(char **old, char *str, int flag)
{
    int     i;
    int     j;
    int     count;
    char    **new;
    char    **words;

    i = 0;
    while (old && old[i])
        i++;
    count = 0;
    words = split_new(str, &count, flag);
    new = ft_calloc(sizeof(char *), i + count + 1);
    j = 0;
    new = append_old_arr(old, &j, i, count);
    if (flag)
    {
        i = 0;
        while (words && words[i])
            new[j++] = ft_strdup(words[i++]);
    }
    else
        new[j++] = ft_strdup(str);
    new[j] = NULL;
    return (new);
}

//void    create_new_field(char **field, char **org, int j, char *exp)
//{
//    char    *new;
//    int     i;
//    int     count;
    
//    new = NULL;
//    i = 0;
//    new = ft_strjoin(new, take_before_wildcard(*field, org, j, &i));
//    count = get_new_len(exp);
//    new = ft_strjoin(new, update_new(count));
//    new = ft_strjoin(new, take_after_wildcard(*field, org, j, &i));
//    *field = new;
//}

int get_new_len(const char *exp)
{
    int i = 0;
    int count = 0;

    while (exp && exp[i])
    {
        if (exp[i] != ' ')
            count++;
        i++;
    }
    return count;
}

char *update_new(int len)
{
    char *res = ft_malloc(1, len + 1);
    if (!res)
        return NULL;

    for (int i = 0; i < len; i++)
        res[i] = '6';

    res[len] = '\0';
    return res;
}

static char    *take_after_wildcard(char *field, char **org, int j, int *i)
{
    int     offset;
    int     total_len;
    char    *res;

    offset = *i + ft_strlen(org[j]);
    total_len = ft_strlen(field);
    if (offset >= total_len)
        return (ft_strdup(""));
    res = ft_malloc(total_len - offset + 1, 1);
    if (!res)
        return (NULL);
    ft_strlcpy(res, field + offset, total_len - offset + 1);
    return (res);
}


void create_new_field(char **field, char **org, int j, char *exp)
{
    char    *before;
    char    *middle;
    char    *after;
    char    *new;
    int     i;

    i = 0;
    before = take_before_wildcard(*field, org, j, &i);
    middle = update_new(get_new_len(exp));
    after = take_after_wildcard(*field, org, j, &i);
    new = ft_strjoin(before, middle);
    before = new;
    new = ft_strjoin(before, after);
    *field = new;
}

int     get_len(char **str)
{
    int     count;

    count = 0;
    while (str && str[count])
        count++;
    return (count);
}

char    **expand_string(char **new, char **field, char **str, int j)
{
    char    *exp;
    char    *old_field;
    char    **arr;
    
    arr = NULL;
    old_field = *field;
    exp = expand_wildcard(field, str[j]);
    if (exp)
    {
        arr = append_to_arr(new, exp, 1);
        create_new_field(field, str, j, exp);
    }
    else
    {
        *field = old_field;
        arr = append_to_arr(new, str[j], 0);
    }
    return (arr);
}

void    call_expand_wildcard(t_array *child, char **field, int i)
{
    char    **str;
    char    **new;
    int     j;

    str = get_string_from_child(child, i);
    new = ft_calloc(sizeof(char *), get_len(str) + 1);
    j = 0;
    while (str && str[j])
    {
        if (search_for(str[j], '*'))
            new = expand_string(new, &field[i], str, j);
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
            call_expand_wildcard(expand->node->children, expand->field_cmd, i);
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
            call_expand_wildcard(expand->node->redirect_list, expand->field_red, i);
		i++;
	}
}

// export a='*' && export b='"' && echo $b$a$b 
// echo "'$err'" --> '$err' but we need this ''
// expo''rt
