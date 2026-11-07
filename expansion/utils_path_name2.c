/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_path_name2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 14:15:26 by salahian          #+#    #+#             */
/*   Updated: 2025/05/18 11:56:19 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int	find_last_slash_before_star(char *str)
{
	int	j;
	int	last_slash;

	j = 0;
	last_slash = -1;
	while (str[j])
	{
		if (str[j] == '/')
			last_slash = j;
		if (str[j] == '*')
			break ;
		j++;
	}
	return (last_slash);
}

char	*check_string_get(char *get)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	while (get[i])
	{
		if (get[i] == '"' || get[i] == '\'')
			tmp = ft_strjoin(tmp, help_check_string_get(get, &i));
		else
		{
			tmp = append_char(tmp, get[i++]);
		}
	}
	return (tmp);
}

char	*expand_wild(char **field, char *path, char *get)
{
	struct dirent	*file;
	DIR				*dir;
	char			*new_str;
	char			*add;

	if (path)
		dir = opendir(path);
	else
		dir = opendir(".");
	if (!dir)
		return (NULL);
	new_str = NULL;
	file = readdir(dir);
	if (check_string_is_not_null(get))
		get = check_string_get(get);
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
			continue ;
		}
		if (!quote && s[i] == '*')
			return (0);
		if (s[i])
			i++;
	}
	return (1);
}

void	get_pattern_and_path(char *str, char **path, char **pattern)
{
	char	*slash;
	char	*wild;
	int		last_slash;

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
