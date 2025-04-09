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

t_ast_node *ast_root(t_token *tkn)
{
	t_ast_node *curr_node;
	curr_node = creat_ast_node(AST_COMPOUNED_CMD);
	while (true)
	{
		if (tkn->type != AST_AND && tkn->type != T)
		append_childs(tkn, curr_node);
	}
	return (root_node);
}

t_ast_node	*parse_tokens(t_token *tokens)
{
	t_ast_node *compound_node = ast_root(tokens);
	// this root hase childs and those child ,
	// are pipelines each pipe line hase childs that are cmds ,
	// and cmds are either a simple cmd or a subshell
	t_ast_node *current_node = NULL;

	while (tokens && is_eof(tokens) == false)
	{
		// now i should use a function taht see the command as a pipelines of type cmd_list

		// Check if the token is a command (WORD).
		// if (tkn_is_redirection(&tokens))
		// {
		// 	current_node = redirection(&tokens);
		// }
		// else if (tokens->type == TOKEN_WORD )
		// {
		// 	// becuse a simple cmd is a pipeline sepicial case
		// 	current_node = parse_pipeline(&tokens);
		// }
		// else if ()
		// {

		// }
		// else
		// {
		// 	handle_syntax_error(tokens);
		// 	break ;
		// }

		compound_node = add_to_compound(compound_node, current_node);
	}
	return (compound_node);
}