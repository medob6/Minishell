FLAFS= -Wall -Werror -Wextra -lreadline -fsanitize=address -g #-fsanitize=address
# -lreadline : flag to link readline function
SRC= array.c  create_the_main_list.c     garbage_collector.c  lexem.c  main.c  parsing.c token.c utils_lib/*.c  
CC= cc
NAME=exe


all: $(NAME)

$(NAME):
	@ $(CC) $(SRC) $(FLAFS)  -o $(NAME) 


clean:
	@rm -f $(NAME)

re: clean all