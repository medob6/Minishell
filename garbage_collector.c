/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:53:05 by salahian          #+#    #+#             */
/*   Updated: 2025/05/08 08:43:24 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstclear(t_gar **lst)
{
	t_gar	*d;
	t_gar	*s;

	if (lst == NULL || *lst == NULL)
		return ;
	d = *lst;
	while (d)
	{
		s = d->next;
		free(d->addr);
		free(d);
		d = s;
	}
	*lst = NULL;
}

t_gar   **garbage_list(void)
{
    static t_gar *tail;
    return (&tail);
}

void add_to_the_list(t_gar **tail, t_gar *new)
{
    if (!tail || !new)
        return;
    new->next = *tail;
    *tail = new;
}

void panic(char *err)
{
    ft_putendl_fd(err,1);
	ft_lstclear(garbage_list());
    exit(1);
}

void    *ft_malloc(int size, int bytes)
{
    void    *p;
    t_gar   *new;

    new = malloc(sizeof(t_gar));
    if (!new)
        panic("Error:\nmalloc failed\n");
    new->addr = malloc(size * bytes);
    if (!new->addr)
        return (free(new), NULL);
    p = new->addr;
    new->next = NULL;
    add_to_the_list(garbage_list(), new);
    return(p);
}
