/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 08:41:18 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/16 10:05:09 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

char	*get_next_line(int fd);
char	*jbad_line(char **res, size_t len, int nline);
char	*ft_str(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);

#endif