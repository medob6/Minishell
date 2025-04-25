FLAFS= -Wall -Werror -Wextra -lreadline -g -fsanitize=address
# -lreadline : flag to link readline function
SRC= array.c  create_the_main_list.c  expand_variables.c   garbage_collector.c  lexem.c  main.c ft_export.c parsing.c token.c field_splitting.c  removes_qouts.c expand_path_name.c utils_lib/*.c  
CC= cc
NAME=bash_prompt


all: $(NAME)

$(NAME):
	@ $(CC) $(SRC) $(FLAFS)  -o $(NAME) 


clean:
	@rm -f $(NAME)

re: clean all