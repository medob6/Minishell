/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:49:52 by salahian          #+#    #+#             */
/*   Updated: 2025/04/29 14:21:55 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	check_for_operations(char *cmd_line, int i)
{
	if (cmd_line[i] == '|' && cmd_line[i + 1] == '|')
		return ('o');
	else if (cmd_line[i] == '&' && cmd_line[i + 1] == '&')
		return ('e');
	else if (cmd_line[i] == '>' && cmd_line[i + 1] == '>')
		return ('a');
	else if (cmd_line[i] == '<' && cmd_line[i + 1] == '<')
		return ('h');
	else if (cmd_line[i] == '|')
		return ('|');
	else if (cmd_line[i] == '>')
		return ('>');
	else if (cmd_line[i] == '<')
		return ('<');
	else if (cmd_line[i] == '(')
		return ('(');
	else if (cmd_line[i] == ')')
		return (')');
	return ('\0');
}

char	*get_name_heredoc(void)
{
	char	*new;
	int		fd;

	fd = open("/dev/random", O_RDONLY);
	new = ft_malloc(1, 11);
	read(fd, new, 10);
	new[10] = '\0';
	new = ft_strjoin("/tmp/", new);
	return (new);
}

char    *removes_qouts_heredoc(char *str)
{
    char    *new_str;
    int     j;

    j = 0;
    new_str = ft_strdup("");
    while (str[j])
    {
        if (str[j] == '\'' && check_for_next_one(str, j))
            j = take_inside_qout(&new_str, str, j);
        else if (str[j] == '"' && check_for_next_one(str, j))
            j = take_inside_qout(&new_str, str, j);
        else
            new_str = append_char(new_str, str[j++]);
    }
	return (new_str);
}

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new;

	new = ft_malloc(sizeof(t_token), 1);
	// printf("%s\n",value);
	new->value.str_value = ft_strdup(value);
	new->value.fd_value = -1;
	new->value.theres_qouts = 0;
	new->type = type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	append_token(t_token **head, t_token **tail, t_token *new)
{
	if (!new)
		return ;
	if (!*head)
		*head = new;
	else
	{
		(*tail)->next = new;
		new->prev = *tail;
	}
	*tail = new;
}

int	handle_operator(t_token **head, t_token **tail, char c)
{
	t_token	*new;

	new = NULL;
	if (c == 'o')
		new = create_token("||", TOKEN_OR);
	else if (c == '|')
		new = create_token("|", TOKEN_PIPE);
	else if (c == 'e')
		new = create_token("&&", TOKEN_AND);
	else if (c == '(')
		new = create_token("(", TOKEN_PARENTESIS_OPEN);
	else if (c == ')')
		new = create_token(")", TOKEN_PARENTESIS_CLOSE);
	append_token(head, tail, new);
	return ((c == 'o' || c == 'a' || c == 'h' || c == 'e') + 1);
}

void	create_simple_token(t_token **head, t_token **tail, char *s)
{
	t_token	*new;

	new = create_token(s, TOKEN_WORD);
	append_token(head, tail, new);
}
void	print_err(char *err, char *str)
{
	char	*buffer;
	char	*tmp;

	buffer = ft_strjoin("minishell: ", err);
	tmp = buffer;
	buffer = ft_strjoin(tmp, ": ");
	free(tmp);
	tmp = buffer;
	buffer = ft_strjoin(tmp, str);
	free(tmp);
	tmp = buffer;
	buffer = ft_strjoin(tmp, "\n");
	free(tmp);
	ft_putstr_fd(buffer, 2);
	free(buffer);
}
void	handle_error(char	*str)
{
	print_err(strerror(errno), str);
	exit(1);
}

int	create_temp_file(int *old_fd)
{
	int	fd1;
	char	*str;

	str = get_name_heredoc();
	fd1 = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	*old_fd = open(str, O_RDONLY);
	unlink(str);
	if (fd1 == -1 || *old_fd == -1)
		handle_error(str);
	return (fd1);
}

void	print_err1(char *err, char *str)
{
	char	*buffer;
	char	*tmp;

	buffer = ft_strjoin("\nMysh: ", err);
	tmp = buffer;
	buffer = ft_strjoin(tmp, str);
	free(tmp);
	tmp = buffer;
	buffer = ft_strjoin(tmp, "')\n");
	free(tmp);
	ft_putstr_fd(buffer, 2);
	free(buffer);
}

int	is_delemeter(char *line, char *delemeter)
{
	if (!ft_strcmp(line, delemeter))
		return (1);
	return (0);
}

void	process_input(int fd1, char *delemeter)
{
	char	*line;
	char	*tmp;

	line = NULL;
	while (1)
	{
		if (line && is_delemeter(line, delemeter))
			break ;
		if (line)
		{
			tmp = ft_strjoin(line, "\n");
			free(line);
			write(fd1, tmp, ft_strlen(tmp));
			free(tmp);
		}
		line = readline("> ");
		if (!line)
			break ;
	}
	if (!line)
		print_err1("warning: here-document delimited by end-of-file (wanted `",
			delemeter);
	free(line);
}

void	read_from_herdoc(char *delemeter, int *old_fd)
{
	int	fd1;

	fd1 = create_temp_file(old_fd);
	process_input(fd1, delemeter);
	close(fd1);
}

int	handle_herdoc(char *delimiter)
{
	int	fd;

	fd = -1;
	if (!delimiter)
		return (0);
	read_from_herdoc(delimiter, &fd);
	return (fd);
}

void	append_redirect_arg(t_token **head, t_token **tail, char c, char *next)
{
	if (c == '>')
		append_token(head, tail, create_token(next, TOKEN_REDIRECT_OUT));
	else if (c == 'a')
		append_token(head, tail, create_token(next, TOKEN_APPEND));
	else
		append_token(head, tail, create_token(next, TOKEN_REDIRECT_IN));
}

void	append_empty_redirect(t_token **head, t_token **tail, char c)
{
	if (c == '>')
		append_token(head, tail, create_token(NULL, TOKEN_REDIRECT_OUT));
	else if (c == 'a')
		append_token(head, tail, create_token(NULL, TOKEN_APPEND));
	else
		append_token(head, tail, create_token(NULL, TOKEN_REDIRECT_IN));
}

int	handle_redirection(t_token **head, t_token **tail, char c, char *next)
{
	char	nc;

	if (next)
	{
		nc = check_for_operations(next, 0);
		if (nc == '\0')
		{
			append_redirect_arg(head, tail, c, next);
			return (1);
		}
		else
			append_empty_redirect(head, tail, c);
	}
	else
		append_empty_redirect(head, tail, c);
	return (0);
}

int		check_for_q(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

int	handle_heredoc_case(t_token **head, t_token **tail, char *next)
{
	t_token	*new_token;
	int		fd;
	int		qouts;

	qouts = 0;
	if (next && check_for_operations(next, 0) == '\0')
	{
		if (check_for_q(next))
		{
			qouts = 1;
			next = removes_qouts_heredoc(next);
		}
		fd = handle_herdoc(next);
		new_token = create_token(next, TOKEN_HEREDOC);
		new_token->value.fd_value = fd;
		// printf("fd = %d\n",fd);
		// close(fd);
		if (qouts)
			new_token->value.theres_qouts = 1;
		append_token(head, tail, new_token);
		return (1);
	}
	append_token(head, tail, create_token(NULL, TOKEN_HEREDOC));
	return (0);
}

void	close_all_files(t_token **head)
{
	t_token *tmp;
	
	tmp = *head;
	while (tmp)
	{
		if (tmp->value.fd_value > -1)
			close(tmp->value.fd_value);
		tmp = tmp->next;
	}
}

void  check_the_string(t_token **head, t_token **tail, char **s, int *index)
{
	int		i;
	char	c;
	static int heredoc;
	char	*next;

	i = 0;
	next = s[*index + 1];
	while (s[*index][i])
	{
		if (s[*index][i] == '\'' || s[*index][i] == '\"')
		{
			create_simple_token(head, tail, s[*index]);
			return ;
		}
		c = check_for_operations(s[*index], i);
		if (c)
		{
			if (c == '|' || c == '(')
				heredoc = 0;
			if (c == 'h')
			{
				heredoc++;
				if (heredoc > 16)
				{
					ft_print("bash: maximum here-document count exceeded\n", 2);
					heredoc = 0;
					close_all_files(head);
					exit(2);
				}
				*index += handle_heredoc_case(head, tail, next);
			}
			else
			{
				i += handle_operator(head, tail, c);
				if (c == '>' || c == '<' || c == 'a')
					*index += handle_redirection(head, tail, c, next);
			}
			return ;
		}
		i++;
	}
	create_simple_token(head, tail, s[*index]);
}

int	counter(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

t_token	**create_tokens(char **str)
{
	int		i;
	t_token	**tokens;
	t_token	*head;
	t_token	*tail;

	head = NULL;
	tail = NULL;
	if (!str)
		return (NULL);
	tokens = ft_malloc(sizeof(t_token *), 2);
	i = 0;
	while (str[i])
	{
		check_the_string(&head, &tail, str, &i);
		i++;
	}
	append_token(&head, &tail, create_token(NULL, TOKEN_EOF));
	tokens[0] = head;
	tokens[1] = NULL;
	return (tokens);
}
