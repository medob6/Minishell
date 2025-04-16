/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 09:48:30 by mbousset          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/09 15:17:58 by salahian         ###   ########.fr       */
=======
/*   Updated: 2025/04/15 16:02:32 by salahian         ###   ########.fr       */
>>>>>>> b83116466f80f973f0d96241032b11d51b679a10
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*str;
	int		i;

	if (!s1)
		return (NULL);
	str = ft_malloc((ft_strlen(s1) + 1), sizeof(char));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
