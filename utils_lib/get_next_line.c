/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:19:10 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/16 22:10:49 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	read_fails(int n, char **rest)
{
	if (n == -1)
	{
		if (*rest != NULL)
			ft_free(*rest);
		*rest = NULL;
		return (1);
	}
	return (0);
}

char	*checkline_in_rest(char **rest)
{
	int	i;

	i = 0;
	if (*rest)
	{
		while ((*rest)[i] && (*rest)[i] != '\n')
			i++;
		if ((*rest)[i] == '\n')
			return (jbad_line(rest, i, 1));
	}
	return (NULL);
}

char	*proccess_buffer(int fd, char *buf, char **res)
{
	char	*temp;
	int		n;
	int		i;

	n = 1;
	while (n > 0)
	{
		n = read(fd, buf, BUFFER_SIZE);
		if (read_fails(n, res))
			return (NULL);
		buf[n] = '\0';
		temp = *res;
		*res = ft_strjoin(*res, buf);
		ft_free(temp);
		if (!*res)
			return (*res = NULL, NULL);
		i = 0;
		while ((*res)[i] && (*res)[i] != '\n')
			i++;
		if ((*res)[i] == '\n')
			return (jbad_line(res, i, 1));
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*rest;
	char		*buf;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (ft_free(rest), NULL);
	buf = malloc(((size_t)BUFFER_SIZE) + 1);
	if (!buf)
		return (ft_free(rest), rest = NULL, NULL);
	line = checkline_in_rest(&rest);
	if (line)
		return (ft_free(buf), line);
	line = proccess_buffer(fd, buf, &rest);
	if (line)
		return (ft_free(buf), line);
	if (line == NULL && (!rest || *rest == '\0'))
		return (ft_free(buf), ft_free(rest), rest = NULL, NULL);
	line = jbad_line(&rest, ft_strlen(rest), 0);
	ft_free(buf);
	return (line);
}
