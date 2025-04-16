/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:53:05 by salahian          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/09 16:31:55 by salahian         ###   ########.fr       */
=======
/*   Updated: 2025/04/10 16:07:57 by salahian         ###   ########.fr       */
>>>>>>> b83116466f80f973f0d96241032b11d51b679a10
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_gar   **garbage_list(void)
{
    static t_gar *tail;
    return (&tail);
}

void    add_to_the_list(t_gar **tail, t_gar *new)
{
    if (!tail || !*tail)
        *tail = new;
    new->next = *tail;
}

void    *ft_malloc(int size, int bytes)
{
    void    *p;
    t_gar   *new;

    new = malloc(sizeof(t_gar));
    if (!new)
        return (NULL);
<<<<<<< HEAD
    new->add = malloc(size * bytes);
    if (!new->add)
        return (free(new), NULL);
    p = new->add;
=======
    new->addr = malloc(size * bytes);
    if (!new->addr)
        return (free(new), NULL);
    p = new->addr;
>>>>>>> b83116466f80f973f0d96241032b11d51b679a10
    new->next = NULL;
    add_to_the_list(garbage_list(), new);
    return(p);
}