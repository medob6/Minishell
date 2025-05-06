/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:48:59 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/06 13:50:00 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../minishell.h"
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_cmd
{
	char		*path;
	char		**args;
	pid_t		pid;
	int			exit_status;
	t_token		**redirlist;
	t_ast_node	*subshell_node;
	int			is_built_in;
	bool		is_subshell;
}				t_cmd;

typedef struct s_data
{
	int			old_fd;
	int			out_fd;
	int			fd[2];
	t_cmd		*lst_cmd;
	int			cmd_nbr;
	t_env		*env;
}				t_data;

/* Core Execution */
int				execute_cmd_line(t_ast_node *root, t_env *envp);
int				execution(t_ast_node *root, t_env *env);
int				execute_pipeline(t_ast_node *pipeline, t_env *env);
int				execute_subshell(t_ast_node *subshell, t_env *env);
void			execute_cmd(t_cmd cmd, t_data *prg_data);
int				execute_built_in(t_cmd cmd, t_data *data);
void			child(t_data *prg_data, int index);
void			pipe_execution(t_data *prg_data);
bool			should_continue(int status, t_ast_node *op);

/* Command Setup */
bool			cmd_is_path(char *s);
bool			cmd_built_in(char *cmd);
bool			build_cmd(t_cmd *cmd, t_ast_node *node, t_env *env);
t_cmd			*build_cmd_list(int cmd_nbr, t_ast_node **cmd_node,
					t_env *envp);
bool			init_cmd_structure(t_cmd *cmd, t_ast_node *node);
bool			set_cmd_args_and_path(t_cmd *cmd, char **cmd_args, t_env *env);
char			*get_cmd_path(char *cmd, char **envp);

/* Redirection */
void			redirect(t_data *data, t_token *file_obj);
void			perforem_redirections(t_data *data, int n);
void			perforem_subshell_redirs(t_data *data, int n);
void			close_here_docs(t_token **redir_list);
void			reset_fds(t_data *data, int n);
void			save_old_fd(int *old_fd, int *fd);
bool			handle_redirect_token(t_data *data, t_token *token,
					int *last_idx, int idx);
bool			open_output_redirect(t_data *data, t_token *token,
					int *last_idx, int idx);
bool			open_input_redirect(t_data *data, t_token *token);
bool			redirection_builtins(t_data *data, int n);
bool			reopen_last_output(t_data *data, t_token **redir_lst,
					int last_idx);
bool			process_redirections(t_data *data, t_token **redir_lst,
					int *last_idx);
bool			is_ambiguous_redirect(t_token *token);
int				open_file(t_token *file_token);

/* Env and Expansion */
char			**extract_envp(t_env *env);
int				lst_size(t_env *head);
void			expand_pipeline(t_ast_node *node, t_env **env);

/* Built-ins */
int				ft_echo(char **args, int fd);
int				ft_exit(char **args, long last_status);
int				ft_env(t_env **env);
int				ft_unset(char **args, t_env **env);
int				ft_pwd(t_env **env_list);
int				ft_cd(char *path, t_env **env_list);

/* Utilities */
int				ft_strcmp(char *s1, char *s2);
char			*get_value(int type);
char			*get_value_ast(int type);
t_token			**create_tokens(char **str);
void			print_token(t_token *head);
void			handle_missing_cmd(t_cmd cmd, t_data *prg_data);
void			wait_for_prc(t_cmd *cmd_list, int cmd_nbr);
void			print_err(char *err, char *str);
void			exit_status(t_data *prg_data, int status);
void			init_program_data(t_data *data, t_ast_node *pipeline,
					t_env *env);
void			free_garbeg(t_data *prg_data);
void			free_pointers(char **p);
void			ft_free(void *address);
t_gar			**garbage_list(void);
t_env			*create_the_main_list(char **envp);

/* FUNCTIONS */
t_ast_node		*subshell(t_token **token);
t_ast_node		*command(t_token **token);

#endif
