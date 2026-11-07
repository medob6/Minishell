/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:59:20 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/05 18:02:51 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARRAY_H
# define ARRAY_H

# include "../minishell.h"

void	array_extend(t_array *arr);
void	array_push(t_array **list, void *new_item);
t_array	*creat_array(void);

#endif