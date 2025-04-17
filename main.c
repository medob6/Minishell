
#include "minishell.h"


void	ft_lstclear(t_gar **lst)
{
	t_gar	*d;
	t_gar	*s;

	if (lst == NULL || *lst == NULL)
		return ;
	d = *lst;
	while (d)
	{
		s = d->next;
		free(d->addr);
		free(d);
		d = s;
	}
	*lst = NULL;
}

const char	*costruct_prompt(void)
{
	char	*cwd;
	char	*home;
	char	*prompt;
	char	*tmp;

	cwd = ft_malloc(1, 100);
	getcwd(cwd, 100);
	home = getenv("HOME");
	if (home && !ft_strncmp(cwd, home, ft_strlen(home)))
	{
		tmp = ft_strjoin("~", cwd + ft_strlen(home));
		prompt = ft_strjoin(tmp, "$ ");
	}
	else
		prompt = ft_strjoin(cwd, "$ ");
	return (prompt);
}

char	*get_redir_value(int type)
{
	if (type == TOKEN_REDIRECT_IN) // <
		return ("<");
	else if (type == TOKEN_REDIRECT_OUT) // >
		return (">");
	else if (type == TOKEN_APPEND) // >>
		return (">>");
	else if (type == TOKEN_HEREDOC) // <<
		return ("<<");
	else
		return ("fuck you redirections");
}
char	*get_value(int type)
{
	if (type == TOKEN_WORD)
		return ("TOKEN_WORD");
	else if (type == TOKEN_PARENTESIS_OPEN) // (
		return ("TOKEN_PARENTESIS_OPEN");
	else if (type == TOKEN_PARENTESIS_CLOSE) // )
		return ("TOKEN_PARENTESIS_CLOSE");
	else if (type == TOKEN_AND)
		return ("TOKEN_AND");
	else if (type == TOKEN_OR)
		return ("TOKEN_OR");
	else if (type == TOKEN_PIPE) // |
		return ("TOKEN_PIPE");
	else if (type == TOKEN_REDIRECT_IN) // <
		return ("TOKEN_REDIRECT_IN");
	else if (type == TOKEN_REDIRECT_OUT) // >
		return ("TOKEN_REDIRECT_OUT");
	else if (type == TOKEN_APPEND) // >>
		return ("TOKEN_APPEND");
	else if (type == TOKEN_HEREDOC) // <<
		return ("TOKEN_HEREDOC");
	else if (type == TOKEN_EOF)
		return ("TOKEN_EOF");
	else
		return ("UNKNOWN_TOKEN");
}

void	print_token(t_token *head)
{
	while (head)
	{
		printf("%s %s\n", head->value, get_value(head->type));
		head = head->next;
	}
}

int	ft_error(int a)
{
	static int	c;

	if (a != 0)
		c = a;
	return (c);
}

int	ft_print(char *c, int fd)
{
	if (ft_error(0) != -1)
		ft_error(write(fd, c, ft_strlen(c)));
	return (ft_error(0));
}

void	print_lexer(char **s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		printf("token[%d]: [%s]\n", i, s[i]);
		i++;
	}
}
char	*get_value_ast(int type)
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

	if (!node)
		return ;
	for (int i = 0; i < depth; i++)
	{
		if (i < depth)
			printf("\t|");
	}
	printf("_______");
	if (node->type == AST_SIMPLE_CMD)
	{
		printf("AST Node: AST_SIMPLE_COMMAND  arg_list : (");
		if (node->children)
		{
			for (size_t j = 0; j < node->children->length; j++)
				printf(" %s ", (char *)(node->children->items[j]));
		}
		printf(") AND ");
		printf("redirct_list : (");
		if (node->redirect_list)
		{
			for (size_t d = 0; d < node->redirect_list->length; d++)
				printf("  {redir_type : %s, filename : %s} ",
					get_redir_value(((t_token *)node->redirect_list->items[d])->type),
					((t_token *)node->redirect_list->items[d])->value);
		}
		printf(") \n");
		return ;
	}
	else
		printf("AST Node: %s\n", get_value_ast(node->type));
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

int	main(int ac, char **av, char **envp)
{
	char		*cmd_line;
	const char	*prompt;
	t_token		**h;
	t_ast_node	*ast;
	t_env *env;
	(void)ac;
	(void)av;

	ast = NULL;
	ft_error(1);
	prompt = costruct_prompt();
	while (1)
	{
		cmd_line = readline(prompt);
		printf("\n");
		// printf("cmd_line = %s\n",cmd_line);
		if (!cmd_line)
			break ;
		if (*cmd_line)
			add_history(cmd_line);
		h = create_tokens(lexer(cmd_line));
		if (h)
		{
			printf("This are the tokens we have : \n");
			print_token(*h);
			printf("\n\n");
			ast = parse_tokens(*h);
			env = create_the_main_list(envp);
			if (traverse_ast(ast, &env))
				printf("all good\n");
			else
				printf("somthing not right\n");
		}
		if (!ast)
			printf("❌ Parser returned NULL (syntax error?)\n");
		else
			print_ast(ast, 0);
		free(cmd_line);
		rl_on_new_line();
	}
	if (ft_error(0) == -1)
		return (1);
	ft_lstclear(garbage_list());
	rl_clear_history();
	return (0);
}

// example test
// cat << eof  && (echo hello > file1 && cat < file1 | grep hi
		// || echo "fallback") && (ls	-l | grep .c) || mkdir test
		// && echo done >> lolo