#include "minishell.h"

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
	while (true)
	{
		if (is_redirction((*token)->type))
		{
			if (!((*token)->value.str_value))
				return (NULL);
			add_redirect(simple_cmd, *token);
		}
		else if ((*token)->type == TOKEN_WORD)
			add_child(simple_cmd, (*token)->value.str_value);
		else
			break ;
		advance_token(token);
	}
	if ((!simple_cmd->children && !simple_cmd->redirect_list))
		return (NULL);
	return (simple_cmd);
}
bool	paranteses_symetric(t_token **token)
{
	t_token	*temp_head;

	int count_open, count_close;
	count_open = 0;
	count_close = 0;
	temp_head = *token;
	while (temp_head->type != TOKEN_EOF)
	{
		if (temp_head->type == TOKEN_PARENTESIS_OPEN)
			count_open++;
		if (temp_head->type == TOKEN_PARENTESIS_CLOSE)
			count_close++;
		if (count_close > count_open)
			return (false);
		temp_head = (temp_head)->next;
	}
	if (count_close == count_open)
		return (true);
	return (false);
}


t_array *appned_redirections(t_array *redir_lst1,t_array *redir_lst2)
{
	size_t i;

	i = 0;
	if (!redir_lst1)
		return (redir_lst2);
	while (i < redir_lst1->length)
		array_push(&redir_lst2,redir_lst1->items[i++]);
	return (redir_lst2);
}
t_ast_node	*subshell(t_token **token)
{
	t_ast_node	*compouned;

	if ((*token)->type != TOKEN_PARENTESIS_OPEN)
		return (NULL);
	advance_token(token);
	compouned = compound_cmd(token, AST_SUBSHELL);
	if ((*token)->type != TOKEN_PARENTESIS_CLOSE)
		return (NULL);
	else
		advance_token(token);

	while (true)
	{
		if (is_redirction((*token)->type))
		{
			if (!(*token)->value.str_value)
				return (NULL);
			add_redirect(compouned, *token);
		}
		else
			break ;
		advance_token(token);
	}

	if (compouned->children->length == 1) {
	t_ast_node *pipeline = compouned->children->items[0];

	if (pipeline->children->length == 1) {
		t_ast_node *child = pipeline->children->items[0];

		if (child->type == AST_SUBSHELL || child->type == AST_SIMPLE_CMD) {
			if (!compouned->redirect_list) {
				compouned = child;
			} else {
				t_array *redir_list = appned_redirections(child->redirect_list, compouned->redirect_list);
				compouned = child;
				compouned->redirect_list = redir_list;
			}
		}
	}
}

	return (compouned);
}

t_ast_node	*command(t_token **token)
{
	t_ast_node	*command;

	command = subshell(token);
	if (command)
		return (command);
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
bool	is_and_or(t_token_type type)
{
	if (type == TOKEN_AND || type == TOKEN_OR || type == TOKEN_EOF)
		return (true);
	return (false);
}
static t_ast_node	*create_logic_node(t_token **token)
{
	t_ast_node	*logic;

	if ((*token)->type == TOKEN_AND)
		logic = creat_ast_node(AST_AND);
	else
		logic = creat_ast_node(AST_OR);
	advance_token(token);
	return (logic);
}

t_ast_node	*compound_cmd(t_token **token, t_ast_type type)
{
	static int	depth;

	t_ast_node (*compound), (*current);
	depth++;
	compound = creat_ast_node(type);
	while (*token && (*token)->type != TOKEN_EOF)
	{
		current = pipeline(token);
		if (!current || current->type == TOKEN_EOF)
			return (NULL);
		add_child(compound, current);
		if ((*token)->type == TOKEN_AND || (*token)->type == TOKEN_OR)
		{
			current = create_logic_node(token);
			add_child(compound, current);
			if ((*token)->type == TOKEN_EOF)
				return (NULL);
		}
		else
			break ;
	}
	depth--;
	if (depth == 0 && (*token)->type != TOKEN_EOF)
		return (NULL);
	return (compound);
}

// t_token *get_lst_tail(t_token **lst)
// {
// 	t_token *tail;
// 	if (!lst || !*lst)
// 		return (NULL);
// 	tail = *lst;
// 	while (tail->next)
// 		tail = tail->next;
// 	return (tail);
// }

t_ast_node	*parse_tokens(t_token *tokens)
{
	t_ast_node	*root;

	if (!paranteses_symetric(&tokens))
		return (NULL);
	root = compound_cmd(&tokens, AST_COMPOUNED_CMD);
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
