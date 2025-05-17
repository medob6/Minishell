FLAFS= -Wall  -Wextra -Werror -lreadline -g #-fsanitize=address
# -lreadline : flag to link readline function   
SRC=  garbage_collector.c  lexer/*.c  main.c  parsing/*.c utils_lib/*.c  execution/*.c built_ins/*.c expansion/*.c

CC= cc

NAME=minishell

all: $(NAME)

$(NAME):
	@ $(CC) $(SRC) $(FLAFS)  -o $(NAME) 

clean:
	@rm -f $(NAME)

re: clean all
