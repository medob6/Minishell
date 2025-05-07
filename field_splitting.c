/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_splitting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:21:24 by salahian          #+#    #+#             */
/*   Updated: 2025/05/07 09:05:15 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_first_char(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}

int count_nbr_words(char *str, char *field)
{
    int i;
    int in_word;
    int count;

    i = 0;
    in_word = 0;
    count = 0;
    while (str[i])
    {
        if ((str[i] != ' ' && str[i] != '\t') || field[i] != '1')
        {
            if (!in_word)
            {
                in_word = 1;
                count++;
            }
        }
        else
            in_word = 0;
        i++;
    }
    return count;
}

static char *allocat(const char *s, char *field, int *j)
{
    int start;
    int len;
    int i;
    char *w;

    while (s[*j] && (s[*j] == ' ' || s[*j] == '\t') && field[*j] == '1')
        (*j)++;
    start = *j;
    while (s[*j] && ((s[*j] != ' ' && s[*j] != '\t') || field[*j] != '1'))
        (*j)++;
    len = *j - start;
    w = malloc(len + 1);
    if (!w)
        return NULL;
    i = 0;
    while (i < len)
    {
        w[i] = s[start + i];
        i++;
    }
    w[len] = '\0';
    return (w);
}

char **applicate_splitting(char *str, char *field)
{
    int count;
    char **new;
    int j;
    int i;

    count = count_nbr_words(str, field);
    new = ft_calloc(count + 1, sizeof(char *));
    j = 0;
    i = 0;
    while (i < count)
    {
        new[i] = allocat(str, field, &j);
        if (!new[i])
            return NULL;
        i++;
    }
    new[i] = NULL;
    return new;
}


int	check_the_last_arg(char *tmp)
{
	int i;

    i = 0;
    while (tmp && tmp[i] != '$')
        i++;
    if (tmp[i] == '$')
    {
	    if (!check_first_char(tmp[i + 1]))
		    return (0);
    }
    else
    {
        if (!check_first_char(tmp[i]))
		    return (0);
    }
	//i++;
	//while (tmp && tmp[i])
	//{
	//	if (!((tmp[i] >= '0' && tmp[i] <= '9') ||
	//		(tmp[i] >= 'A' && tmp[i] <= 'Z') ||
	//		(tmp[i] >= 'a' && tmp[i] <= 'z') ||
	//		tmp[i] == '_'))
	//		break;
	//	i++;
	//}
	return (1);
}

int skip_single_quotes(const char *s, int i)
{
	char c;

	if (ft_strchr("'", s[i]) != NULL)
	{
		c = s[i];
		i++;
		while (s[i] && s[i] != c)
			i++;
		if (s[i] == c)
			i++;
	}
	return (i);
}

int     check_for_field_split(char *tmp)
{
    int     i;
    int     dollar;
    int     qout;

    i = 0;
    qout = 0;
    dollar = 0;
    while (tmp[i])
    {
        if (tmp[i] == '\'')
            i = skip_single_quotes(tmp, i);
        if (tmp[i] == '"')
        {
            i++;
            while (tmp[i] && tmp[i] != '"')
            {
                if (tmp[i] == '$')
                    return (0);
                i++;
            }
            //i++;
        }
        if (tmp[i] == '$')
            dollar = 1;
        if (!tmp[i])
            break ;
        i++;
    }
    return (dollar);
}

// int     check_for_last_exp(t_ast_node *node)
// {
//     size_t     i;
//     int     j;
//     int     index;
//     int     arr[100];
//     char    *tmp;

//     i = 0;
//     index = 0;
//     while (i < node->children->length)
//     {
//         tmp = (char *)node->children->items[i];
//         j = 0;
//         while (tmp[j])
//         {
//             if (tmp[j] == '$')
//             {
//               arr[index] = i;
//               index++;
//               break;
//             }
//             j++;
//         }
//         i++;
//     }
//     return (arr[index]);
// }

int check_for_last_exp(t_ast_node *node)
{
    size_t i;
    int j;
    int last_index;
    char *tmp;

    last_index = -1;
    i = 0;
    if (!node->children)
        return (last_index);
    while (i < node->children->length)
    {
        tmp = (char *)node->children->items[i];
        j = 0;
        while (tmp[j])
        {
            if (tmp[j] == '$')
            {
                last_index = i;
                break;
            }
            j++;
        }
        i++;
    }
    return (last_index);
}


// int field_splitting(t_ast_node *node)
// {
//     size_t     i;
//     char    *tmp;

//     i = 0;
//     //printf("%d\n", check_for_last_exp(node));
//     tmp = (char *)node->children->items[i];
//     if ((size_t)check_the_last_arg(tmp) != ft_strlen(tmp))
//         return (0);
//     while (i < node->children->length)
//     {
//         tmp = (char *)node->children->items[i];
//         if (check_for_field_split(tmp))
//         {
//             return (1);
//             applicate_field_split(node->children, i);
//         }
//         i++;
//     }
// }