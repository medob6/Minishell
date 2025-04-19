#include "execution.h"

// here i am gona code a set of functions that execute commands

//  TODO IN EXPANSION //

// TODO: expaned before excute ; i exepect that the tree nodes are exepanded before


// TODO IN EXEC //

// TODO: in exection should check if (redirectin list == 1) if not imbigios redirection or any err in redirction
// TODO: in case of a singel simple command in subshell it runs in the parent shell not in a child but built ins behave diferntlly
// TODO: in execution i should first travers on the tree from top to bottom first
// so it will be something like this
// root == parent
// and_or (childerens of root with are pipe and+or ..), each one of them is a child that will retun an exit status , i will take that exit status and check the next nod is || && to determin if i creat the next child
// pipeline = (child of and_or); each pipe line consist of simpel command , each command is a child of pipeline
// simpl_cmd = here we should consume and execut cmd and check of errors like mentioned above

int exec_node(t_ast_node *node) {
  switch (node->type) {
    case AST_COMPOUNED_CMD:
      return exec_and_or(node);
    case AST_PIPELINE:
      return exec_pipeline(node);
    case AST_SIMPLE_CMD:
      return exec_simple_cmd(node);
    case AST_SUBSHELL:
      return exec_subshell(node);
  }
}

void execute_cmd_line(t_ast_node *root, char **envp) {
	int last_status;
	// 1) Expansion pass on the entire tree
	expand_tree(root);

	// error handeling happens in the execution not expansion
	// the root childs are pipelines
	//
	int i;
	int exit_status;
	i = 0;

	while (i < root->children->length)
	{
		exit_status = execute_pipeline(root->children->items[i]);
		if (exit_status != 0 && )

	}

    
}

