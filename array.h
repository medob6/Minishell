#ifndef ARRAY_H
# define ARRAY_H

# include "minishell.h"

// typedef struct s_array
// {
// 	void	**items;
// 	size_t	length;
// 	size_t	capacity;
// }			t_array;

void		array_extend(t_array *arr);
void		array_push(t_array **list, void *new_item);
t_array		*creat_array(void);

#endif