/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 08:44:56 by mbousset          #+#    #+#             */
/*   Updated: 2025/02/26 15:30:03 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*jbad_line(char **res, size_t len, int new_line)
{
	char	*line;
	char	*new_res;
	size_t	res_len;
	size_t	i;

	if (!res || !*res)
		return (NULL);
	line = malloc(len + 1 + new_line);
	if (!line)
		return (free(*res), *res = NULL, NULL);
	i = 0;
	while (i < len + new_line)
	{
		line[i] = (*res)[i];
		i++;
	}
	line[i] = '\0';
	res_len = ft_strlen(*res);
	if (len + 1 < res_len)
		new_res = ft_strjoin(NULL, *res + len + 1);
	else
		new_res = NULL;
	free(*res);
	*res = new_res;
	return (line);
}

// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	size_t	len1;
// 	size_t	len2;
// 	char	*new_str;
// 	size_t	i;
// 	size_t	j;

// 	i = 0;
// 	j = 0;
// 	len1 = 0;
// 	len2 = 0;
// 	if (s1)
// 		len1 = ft_strlen(s1);
// 	if (s2)
// 		len2 = ft_strlen(s2);
// 	new_str = malloc(len1 + len2 + 1);
// 	if (!new_str)
// 		return (NULL);
// 	if (s1)
// 		while (j < len1)
// 			new_str[i++] = s1[j++];
// 	j = 0;
// 	if (s2)
// 		while (j < len2)
// 			new_str[i++] = s2[j++];
// 	return ((new_str[i] = '\0'), new_str);
// }

// size_t	ft_strlen(const char *s)
// {
// 	size_t	n;

// 	n = 0;
// 	while (s && s[n] != '\0')
// 		n++;
// 	return (n);
// }
