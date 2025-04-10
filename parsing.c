#include "parser.h"

// here i should creat code for making my ast
// 1 - we know that ast hase a root ; that we shhould chose
// 2	- the root of the tree has to be chosen carefully ; we do so by searching in our token for the last high priority op

// Note :
// Pipes and Redirections
// The operator with the highest precedence after AND and OR is the PIPE.

// REDIRECTIONS come after the PIPE. The three different redirects don't have to respect any order of priority; they can be equal or different,	it doesn't matter. They can be executed in any order.

// Step 1: Split the command into 2 with the last high priority operator found.

// Step 2: Split the left part into 3 based on the last two similar high priority operators.

// Step 3: Split the leftmost part into 3 based on the last two similar high priority operators.

t_ast_node	*creat_ast_node(int node_type)
{
	t_ast_node	*new_node;

	new_node = malloc(sizeof(t_ast_node));
	new_node->children = NULL;
	new_node->error_message = NULL;
	new_node->redirect_list = NULL;
	new_node->type = node_type;
	return (new_node);
}

t_ast_node	*pipeline(t_token **token)
{
	t_ast_node *pipe_node = create_ast_node(AST_PIPELINE);
	t_ast_node *cmd;

	while (*token && (*token)->type != TOKEN_AND && (*token)->type != TOKEN_OR && (*token)->type != TOKEN_EOF)
	{
		cmd = command(token);
		add_child(pipe_node, cmd);
		if (*token && (*token)->type == TOKEN_PIPE)
			advance_token(token);
		else
			break;
	}
	return (pipe_node);
}

t_ast_node	*ast_root(t_token **token)
{
	t_ast_node *compound = create_ast_node(AST_COMPOUNED_CMD);
	t_ast_node *curr;

	while (*token && (*token)->type != TOKEN_EOF)
	{
		curr = pipeline(token);
		add_child(compound, curr);
		if (*token && ((*token)->type == TOKEN_AND || (*token)->type == TOKEN_OR))
		{
			t_ast_type logic_type = ((*token)->type == TOKEN_AND) ? AST_AND : AST_OR;
			t_ast_node *op_node = create_ast_node(logic_type);
			advance_token(token);
			add_child(compound, op_node);
		}
		else
			break;
	}
	return (compound);
}

t_ast_node *parse_tokens(t_token *tokens)
{
	return ast_root(&tokens);
}

// 	// this root hase childs and those child ,
// 	// are pipelines each pipe line hase childs that are cmds ,
// 	// and cmds are either a simple cmd or a subshell
