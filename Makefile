FLAFS= -Wall  -Wextra -lreadline -fsanitize=address -g 
# -lreadline : flag to link readline function  -Werror -fsanitize=address
SRC= array.c  create_the_main_list.c   garbage_collector.c  lexem.c  main.c  parsing.c token.c utils_lib/*.c  execution/execute_input.c execution/utils_exec.c built_ins/*.c expand_variables.c    field_splitting.c  removes_qouts.c expand_path_name.c
CC= cc
NAME=bash_prompt


all: $(NAME)

$(NAME):
	@ $(CC) $(SRC) $(FLAFS)  -o $(NAME) 


clean:
	@rm -f $(NAME)

re: clean all