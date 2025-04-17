#include "parser.h"

t_ast_node	*creat_ast_node(int node_type)
{
	t_ast_node	*new_node;

	new_node = ft_calloc(1, sizeof(t_ast_node));
	new_node->children = NULL;
	new_node->error_message = NULL;
	new_node->redirect_list = NULL;
	new_node->type = node_type;
	return (new_node);
}

t_ast_node	*simple_command(t_token **token)
{
	t_ast_node	*simple_cmd;

	simple_cmd = creat_ast_node(AST_SIMPLE_CMD);
	while ((*token)->type != TOKEN_EOF)
	{
		if (is_redirction((*token)->type))
		{
			if (!(*token)->value)
				return (NULL);
			add_redirect(simple_cmd, *token);
		}
		else if ((*token)->type == TOKEN_WORD)
			add_child(simple_cmd, (*token)->value);
		else
			break ;
		advance_token(token);
	}
	if ((!simple_cmd->children && !simple_cmd->children))
		return (NULL);
	if ((*token)->type == TOKEN_PARENTESIS_CLOSE)
		advance_token(token);
	return (simple_cmd);
}

bool	paranteses_symetric(t_token **token)
{
	t_token	*temp_head;

	int count_open, count_close;
	count_open = 1;
	count_close = 0;
	if ((*token)->type == TOKEN_PARENTESIS_OPEN)
		advance_token(token);
	temp_head = *token;
	while (temp_head->type != TOKEN_EOF)
	{
		if (temp_head->type == TOKEN_PARENTESIS_OPEN)
			count_open++;
		if (temp_head->type == TOKEN_PARENTESIS_CLOSE)
			count_close++;
		if (count_close == count_open)
		{
			printf("symtric\n");
			return (true);
		}
		printf("open : %d, close : %d \n", count_open, count_close);
		temp_head = (temp_head)->next;
	}
	return (false);
}
t_ast_node	*subshell(t_token **token)
{
	t_ast_node	*compouned;

	// here i check if this current open parantes has a close symtric to it if not return null syntax error else we remove them and call compuned on the current
	if (!paranteses_symetric(token))
	{
		printf("unsymtric\n");
		return (NULL);
	}
	compouned = compound_cmd(token);
	// if ((*token)->type == TOKEN_PARENTESIS_CLOSE)
	// {
	// 	printf("closed\n");
	// 	advance_token(token);
	// 	if ((*token)->type == TOKEN_EOF)
	// 		printf("eof\n");
	// }
	// else
	// 	return (NULL);
	return (compouned);
}

t_ast_node	*command(t_token **token)
{
	if ((*token)->type == TOKEN_PARENTESIS_OPEN)
	{
		// advance_token(token);
		return (subshell(token));
	}
	return (simple_command(token));
}

t_ast_node	*pipeline(t_token **token)
{
	t_ast_node	*pipe_node;
	t_ast_node	*cmd;

	pipe_node = creat_ast_node(AST_PIPELINE);
	while (true)
	{
		cmd = command(token);
		if (!cmd)
			return (NULL);
		add_child(pipe_node, cmd);
		if ((*token)->type != TOKEN_PIPE)
			break ;
		advance_token(token);
	}
	return (pipe_node);
}

t_ast_node	*compound_cmd(t_token **token)
{
	t_ast_node	*compound;
	t_ast_node	*current;
	t_ast_type	logic_type;

	compound = creat_ast_node(AST_COMPOUNED_CMD);
	while (*token && (*token)->type != TOKEN_EOF)
	{
		current = pipeline(token);
		printf("token_type == %s\n", get_value((*token)->type));
		if (!current)
			return (NULL);
		add_child(compound, current);
		if ((*token)->type == TOKEN_AND || (*token)->type == TOKEN_OR)
		{
			if ((*token)->type == TOKEN_AND)
				logic_type = AST_AND;
			else
				logic_type = AST_OR;
			current = creat_ast_node(logic_type);
			add_child(compound, current);
			advance_token(token);
			if ((*token)->type == TOKEN_EOF)
				return (NULL);
		}
		if ((*token)->type != TOKEN_EOF)
			printf("token_type == %s\n", get_value((*token)->type));
	}
	return (compound);
}

t_ast_node	*parse_tokens(t_token *tokens)
{
	t_ast_node	*root;

	root = compound_cmd(&tokens);
	if (!root)
		return (NULL);
	return (root);
}

// ////////////////////////// helper functions ////////////////////// //

void	advance_token(t_token **token)
{
	if (token && *token)
		*token = (*token)->next;
}

void	add_child(t_ast_node *parent, void *new_child)
{
	array_push(&parent->children, new_child);
}

void	add_redirect(t_ast_node *parent, t_token *redir)
{
	array_push(&parent->redirect_list, redir);
}

bool	is_redirction(t_token_type token_type)
{
	if (token_type == TOKEN_REDIRECT_IN || token_type == TOKEN_APPEND
		|| token_type == TOKEN_REDIRECT_OUT || token_type == TOKEN_HEREDOC)
		return (true);
	return (false);
}
