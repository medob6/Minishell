/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:06:38 by salahian          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/09 15:18:20 by salahian         ###   ########.fr       */
=======
/*   Updated: 2025/04/15 16:02:54 by salahian         ###   ########.fr       */
>>>>>>> b83116466f80f973f0d96241032b11d51b679a10
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char    *ft_substr(char const *s, unsigned int start, size_t len)
{
        char    *p;
        size_t  i;
        size_t  j;
        size_t  e;

        if (s == NULL)
                return (NULL);
        e = ft_strlen(s);
        if (start >= e)
                return (ft_strdup(""));
        if (len > e - start)
                len = e - start;
        i = (size_t)start;
        p = ft_malloc(sizeof(char), (len + 1));
        if (p == NULL)
                return (NULL);
        j = 0;
        while (j < len && (s[i] != '\0'))
        {
                p[j] = s[i];
                i++;
                j++;
        }
        p[j] = '\0';
        return (p);
}
