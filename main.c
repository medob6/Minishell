
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
		tmp = ft_strjoin("\033[1;32m~\033[0m", cwd + ft_strlen(home));
		prompt = ft_strjoin(tmp, "\033[1;34m$ \033[0m");
	}
	else
		prompt = ft_strjoin(cwd, "\033[1;34m$ \033[0m");
	return (prompt);
}

char	*get_redir_value(int type)
{
	if (type == TOKEN_REDIRECT_IN)
		return ("<");
	else if (type == TOKEN_REDIRECT_OUT)
		return (">");
	else if (type == TOKEN_APPEND)
		return (">>");
	else if (type == TOKEN_HEREDOC)
		return ("<<");
	else
		return ("\033[1;31mfuck you redirections\033[0m");
}

char	*get_value(int type)
{
	if (type == TOKEN_WORD)
		return ("\033[1;34mTOKEN_WORD\033[0m");
	else if (type == TOKEN_PARENTESIS_OPEN)
		return ("\033[1;34mTOKEN_PARENTESIS_OPEN\033[0m");
	else if (type == TOKEN_PARENTESIS_CLOSE)
		return ("\033[1;34mTOKEN_PARENTESIS_CLOSE\033[0m");
	else if (type == TOKEN_AND)
		return ("\033[1;33mTOKEN_AND\033[0m");
	else if (type == TOKEN_OR)
		return ("\033[1;33mTOKEN_OR\033[0m");
	else if (type == TOKEN_PIPE)
		return ("\033[1;36mTOKEN_PIPE\033[0m");
	else if (type == TOKEN_REDIRECT_IN)
		return ("\033[1;35mTOKEN_REDIRECT_IN\033[0m");
	else if (type == TOKEN_REDIRECT_OUT)
		return ("\033[1;35mTOKEN_REDIRECT_OUT\033[0m");
	else if (type == TOKEN_APPEND)
		return ("\033[1;35mTOKEN_APPEND\033[0m");
	else if (type == TOKEN_HEREDOC)
		return ("\033[1;35mTOKEN_HEREDOC\033[0m");
	else if (type == TOKEN_EOF)
		return ("\033[1;90mTOKEN_EOF\033[0m");
	else
		return ("\033[1;31mUNKNOWN_TOKEN\033[0m");
}

void	print_token(t_token *head)
{
	while (head)
	{
		printf("   \033[0;36m%-15s => %s\033[0m\n", head->value, get_value(head->type));
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
	int	i = 0;

	while (s && s[i])
	{
		printf("\033[0;33mtoken[%d]:\033[0m [\033[0;32m%s\033[0m]\n", i, s[i]);
		i++;
	}
}

char	*get_value_ast(int type)
{
	if (type == AST_COMPOUNED_CMD)
		return ("\033[0;35mAST_COMPOUNED_CMD\033[0m");
	else if (type == AST_CMD)
		return ("\033[0;35mAST_CMD\033[0m");
	else if (type == AST_SIMPLE_CMD)
		return ("\033[0;35mAST_SIMPLE_CMD\033[0m");
	else if (type == AST_PIPELINE)
		return ("\033[0;35mAST_PIPELINE\033[0m");
	else if (type == AST_AND)
		return ("\033[1;33mAST_AND\033[0m");
	else if (type == AST_OR)
		return ("\033[1;33mAST_OR\033[0m");
	else if (type == AST_SUBSHELL)
		return ("\033[1;36mAST_SUBSHELL\033[0m");
	else if (type == AST_ERROR)
		return ("\033[1;31mAST_ERROR\033[0m");
	else
		return ("\033[1;90margment\033[0m");
}

void	print_redir(t_ast_node *node)
{
	if (!node || !node->redirect_list)
	{
		printf("\n");
		return ;
	}
	printf("\033[0;36mredir_list:\033[0m (");
	for (size_t d = 0; d < node->redirect_list->length; d++)
	{
		t_token *redir = (t_token *)node->redirect_list->items[d];
		printf(" \033[0;36m{redir_type: \033[1;36m%s\033[0;36m, filename: \033[1;36m%s\033[0;36m}\033[0m ",
			get_redir_value(redir->type),
			redir->value);
	}
	printf(")\n");
}

void	print_tabs(int n)
{
	while (n--)
		printf("\t");
}

void	print_ast(t_ast_node *node, int depth)
{
	t_token	*redir;

	if (!node)
		return ;
	for (int i = 0; i < depth; i++)
		printf("\033[0;32m\t|\033[0m");
	printf("\033[0;32m_______\033[0m");
	if (node->type == AST_SIMPLE_CMD)
	{
		printf("\033[0;31mAST Node:\033[0m AST_SIMPLE_COMMAND ");
		printf("\033[0;34marg_list:\033[0m (");
		if (node->children)
		{
			for (size_t j = 0; j < node->children->length; j++)
				printf("\033[0;34m%s\033[0m",
					(char *)(node->children->items[j]));
		}
		printf(") ; ");
		printf("\033[0;36mredir_list:\033[0m (");
		if (node->redirect_list)
		{
			for (size_t d = 0; d < node->redirect_list->length; d++)
			{
				redir = (t_token *)node->redirect_list->items[d];
				printf(" \033[0;36m{redir_type: %s, filename: %s}\033[0m ",
					get_redir_value(redir->type), redir->value);
			}
		}
		printf(")\n");
		return ;
	}
	else
	{
		printf("\033[0;31mAST Node:\033[0m %s", get_value_ast(node->type));
		if (node->type == AST_SUBSHELL)
		{
			printf(" ; ");
			print_redir(node);
		}
		printf("\n");
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
		printf("\033[0;36mcmd_line is:\033[0m  \033[1;37m%s\033[0m\n\n", cmd_line);
		if (!cmd_line)
			break ;
		if (*cmd_line)
			add_history(cmd_line);
		h = create_tokens(lexer(cmd_line));
		if (h)
		{
			printf("\033[0;32m============================\033[0m\n\n");
			printf("\033[1;33m📦 Tokens:\033[0m\n\n");
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
		{
			printf("\033[0;32m============================\033[0m\n\n");
			printf("❌ \033[1;31mParser returned NULL (syntax error?)\033[0m\n");
		}
		else
		{
			printf("\033[0;32m============================\033[0m\n\n");
			printf("\033[1;34m🌳 This is the AST:\033[0m\n\n");
			print_ast(ast, 0);
		}
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
// cat << eof  && (echo hello > file1 && cat < file1 | grep hi || echo "fallback") && (ls	-l | grep .c) || mkdir test && echo done >> lolo