FLAFS= -Wall  -Wextra -Werror -lreadline -g 
# -lreadline : flag to link readline function   -fsanitize=address
SRC=  garbage_collector.c  lexer/*.c  main.c  parsing/*.c utils_lib/*.c  execution/*.c built_ins/*.c expansion/*.c
CC= cc
NAME=bash_prompt

all: $(NAME)

$(NAME):
	@ $(CC) $(SRC) $(FLAFS)  -o $(NAME) 

clean:
	@rm -f $(NAME)

re: clean all