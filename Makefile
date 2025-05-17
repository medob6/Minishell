FLAGS= -Wall -Wextra -Werror -lreadline -g

SRC=  garbage_collector.c main.c \
      built_ins/create_the_main_list.c built_ins/env_helpers.c built_ins/export_helper1.c \
      built_ins/export_helper2.c built_ins/ft_cd_pwd.c built_ins/ft_echo.c \
      built_ins/ft_env.c built_ins/ft_exit.c built_ins/ft_export.c built_ins/ft_unset.c \
      execution/cmd_builder.c execution/cmd_utils.c execution/execute_cmd.c \
      execution/execution_flow.c execution/execution_main.c execution/garbeg_handlers.c \
      execution/helper_funcs.c execution/path_build.c execution/redir_helpers.c \
      execution/redirect_utils.c execution/redirections.c \
      expansion/create_array.c expansion/create_field.c expansion/create_field_red.c \
      expansion/expand_herdoc.c expansion/expand_path_name.c expansion/expand_variables.c \
      expansion/field_splitting.c expansion/remove_empty_strings.c expansion/removes_qouts.c \
      expansion/update_field1.c expansion/update_field.c expansion/utils_path_name.c \
      expansion/utils_path_name1.c expansion/utils_path_name2.c \
      expansion/utils_variables.c expansion/utils_variables1.c expansion/utils_variables2.c \
      lexer/handle_heredoc.c lexer/handle_heredoc1.c lexer/handle_heredoc2.c \
      lexer/lexem.c lexer/token.c lexer/utils_lexem.c lexer/utils_lexem1.c \
      lexer/utils_lexem2.c lexer/utils_token.c lexer/utils_token1.c \
      parsing/array.c parsing/pars_cmd_utils.c parsing/parsing.c \
      parsing/parsing_utils.c parsing/subshell.c \
      utils_lib/ft_atoi.c utils_lib/ft_bzero.c utils_lib/ft_calloc.c \
      utils_lib/ft_isdigit.c utils_lib/ft_itoa.c utils_lib/ft_putchar_fd.c \
      utils_lib/ft_putendl_fd.c utils_lib/ft_putstr_fd.c utils_lib/ft_split.c \
      utils_lib/ft_strchr.c utils_lib/ft_strcmp.c utils_lib/ft_strdup.c \
      utils_lib/ft_strjoin.c utils_lib/ft_strlen.c utils_lib/ft_strncmp.c \
      utils_lib/ft_substr.c utils_lib/ft_tolower.c utils_lib/get_next_line.c \
      utils_lib/get_next_line_utils.c

CC= cc

NAME= minishell

all: $(NAME)

$(NAME):
	@$(CC) $(SRC) $(FLAGS) -o $(NAME)

clean:
	@rm -f $(NAME)

fclean: clean
	@rm -f $(NAME)

re: fclean all
