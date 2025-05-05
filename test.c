#include "parsing/parser.h"

t_token	*new_token(t_token_type type, char *value)
{
	t_token	*tok;

	tok = ft_calloc(1, sizeof(t_token));
	tok->type = type;
	tok->value.str_value = value ? strdup(value) : NULL;
	tok->next = NULL;
	return (tok);
}
void	add_token_to_list(t_token **head, t_token **tail, t_token_type type,
		char *value)
{
	t_token	*new;

	new = new_token(type, value);
	if (!*head)
	{
		*head = new;
		*tail = new;
	}
	else
	{
		(*tail)->next = new;
		*tail = new;
	}
}
char	*get_value(int type)
{
	if (type == AST_COMPOUNED_CMD)
		return ("AST_COMPOUNED_CMD");
	else if (type == AST_CMD)
		return ("AST_CMD");
	else if (type == AST_SIMPLE_CMD)
		return ("AST_SIMPLE_CMD");
	else if (type == AST_PIPELINE)
		return ("AST_PIPELINE");
	else if (type == AST_AND)
		return ("AST_AND");
	else if (type == AST_OR)
		return ("AST_OR");
	else if (type == AST_SUBSHELL)
		return ("AST_SUBSHELL");
	else if (type == AST_ERROR)
		return ("AST_ERROR");
	else
		return ("argment");
}
void	print_ast(t_ast_node *node, int depth)
{
	t_token	*redir;

	if (!node && node->type == AST_SIMPLE_CMD)
		return ;
	for (int i = 0; i < depth; i++)
	{
		if (i < depth)
			printf("\033[0;32m\t|\033[0m");
	}
	printf("\033[0;32m_______\033[0m");
	printf("AST Node: %s\n", get_value(node->type));
	if (node->redirect_list)
	{
		for (size_t i = 0; i < node->redirect_list->length; i++)
		{
			redir = node->redirect_list->items[i];
			for (int j = 0; j < depth + 1; j++)
				printf("  ");
			printf("Redirect: %s\n", redir->value.str_value);
		}
	}
	if (node->children)
	{
		for (size_t i = 0; i < node->children->length; i++)
			print_ast(node->children->items[i], depth + 1);
	}
}
