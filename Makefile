FLAFS= -Wall -Werror -Wextra -lreadline -fsanitize=address -g #-fsanitize=address
# -lreadline : flag to link readline function
SRC= main.c lexem.c token.c utils_lib/*.c
CC= cc
NAME=exe


all: $(NAME)

$(NAME):
	@ $(CC) $(SRC) $(FLAFS)  -o $(NAME) 


clean:
	@rm -f $(NAME)

re: clean all