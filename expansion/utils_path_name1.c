/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_path_name1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 14:12:59 by salahian          #+#    #+#             */
/*   Updated: 2025/05/18 11:56:37 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int	check_after_slash(char *get, int slash)
{
	while (get[slash])
	{
		if (get[slash] != '/')
			return (0);
		slash++;
	}
	return (1);
}

char	*handle_dir(struct dirent *dir, char *get, char *path, char *field)
{
	int		slash;
	int		header;
	char	*tmp;
	char	*new;

	header = (get[0] == '.');
	slash = find_last_slash(get);
	if (slash != -1 && check_after_slash(get, slash))
		new = ft_substr(get, 0, slash);
	else
		new = ft_strdup(get);
	tmp = ft_strdup(dir->d_name);
	if (((header && dir->d_name[0] == '.') || (dir->d_name[0] != '.'))
		&& match_pattern(field, new, dir->d_name))
	{
		if (slash != -1)
			return (ft_strjoin(ft_strjoin(path, tmp), "/ "));
		return (ft_strjoin(ft_strjoin(path, tmp), " "));
	}
	return (NULL);
}

char	*help_expand_wild(struct dirent *dir, char *path, char *get,
		char *field)
{
	char	*tmp;
	char	*result;

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

char	*check_string_is_not_null(char *get)
{
	int		i;
	char	*tmp;
	char	c;

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

char	*help_check_string_get(char *get, int *i)
{
	char	c;
	char	*tmp;

	tmp = NULL;
	c = get[(*i)++];
	while (get[*i] && get[*i] != c)
	{
		tmp = append_char(tmp, get[(*i)++]);
	}
	if (get[*i] == c)
		(*i)++;
	return (tmp);
}
