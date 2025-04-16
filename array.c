#include "array.h"

t_array	*creat_array()
{
	t_array *new_array;
	new_array = malloc(sizeof(t_array));
	new_array->capacity = 2;
	new_array->length = 0;
	new_array->items = malloc(sizeof(void *) * new_array->capacity);
	return (new_array);
}

void	array_extend(t_array *arr)
{
	void	**new_items;
	size_t		i;

	arr->capacity *= 2;
	new_items = malloc(sizeof(void *) * arr->capacity);
	i = 0;
	while (i < arr->length)
	{
		new_items[i] = arr->items[i];
		i++;
	}
	arr->items = new_items;
}

void	array_push(t_array **list, void *new_item)
{
	if (!*list)
		*list = creat_array();
	if ((*list)->length >= (*list)->capacity)
		array_extend(*list);
	(*list)->items[(*list)->length] = new_item;
	(*list)->length++;
}
