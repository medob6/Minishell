/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:28:46 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/16 22:32:02 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include "../minishell.h"
# include <limits.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

int		ft_isdigit(int c);
int		ft_atoi(char *str);
int		ft_strcmp(char *s1, char *s2);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1);
char	*ft_strchr(const char *s, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
void	ft_bzero(void *s, size_t n);
int		ft_tolower(int c);
void	*ft_calloc(size_t count, size_t size);
char	**ft_split(const char *str, char c);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
char	*fill_word(const char *src, int start, int end);
void	free_all(char **p);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_itoa(long n);

#endif
