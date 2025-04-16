#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// AST_COMPOUNED_CMD,
// AST_CMD,
// AST_SIMPLE_CMD,
// AST_PIPELINE, // command | command | ...
// AST_AND,      // command && command
// AST_OR,       // command || command
// AST_SUBSHELL, // ( command_list )
// AST_ERROR     // for syntax errors

t_token	*new_token(t_token_type type, char *value)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	tok->type = type;
	tok->value = value ? strdup(value) : NULL;
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
			printf("\t|");
	}
	printf("_______");
	printf("AST Node: %s\n", get_value(node->type));
	if (node->redirect_list)
	{
		for (size_t i = 0; i < node->redirect_list->length; i++)
		{
			redir = node->redirect_list->items[i];
			for (int j = 0; j < depth + 1; j++)
				printf("  ");
			printf("Redirect: %s\n", redir->value);
		}
	}
	if (node->children)
	{
		for (size_t i = 0; i < node->children->length; i++)
			print_ast(node->children->items[i], depth + 1);
	}
}

t_token	*tokenize_test_input(char **token)
{
	t_token	*head;
	t_token	*tail;

	head = NULL;
	tail = NULL;
	// this are the tokens types :
	// AST_COMPOUNED_CMD,
	// AST_CMD,
	// AST_SIMPLE_CMD,
	// AST_PIPELINE, // command | command | ...
	// AST_AND,      // command && command
	// AST_OR,       // command || command
	// AST_SUBSHELL, // ( command_list )
	// AST_ERROR     // for syntax errors
	// example test "echo hello > > | ls -l > out"
	add_token_to_list(&head, &tail, TOKEN_WORD, "ls");
	add_token_to_list(&head, &tail, TOKEN_WORD, "-l");
	add_token_to_list(&head, &tail, TOKEN_PIPE, NULL);
	add_token_to_list(&head, &tail, TOKEN_WORD, "grep");
	add_token_to_list(&head, &tail, TOKEN_WORD, "txt");
	add_token_to_list(&head, &tail, TOKEN_AND, NULL);
	add_token_to_list(&head, &tail, TOKEN_WORD, "echo");
	add_token_to_list(&head, &tail, TOKEN_WORD, "done");
	// add_token_to_list(&head, &tail, TOKEN_AND, NULL);
	// add_token_to_list(&head, &tail, TOKEN_PIPE, NULL);
	add_token_to_list(&head, &tail, TOKEN_EOF, NULL);
	return (head);
}

int	main(int ac, char **av)
{
	t_token		*tokens;
	t_ast_node	*ast;

	tokens = tokenize_test_input(av);
	if (tokens)
		printf("token created .\n");
	ast = parse_tokens(tokens);
	if (!ast)
		printf("❌ Parser returned NULL (syntax error?)\n");
	else
		print_ast(ast, 0); // you'll need to implement a small tree printer
	return (0);
}
