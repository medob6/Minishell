FLAFS= -Wall -Werror -Wextra -lreadline #-fsanitize=address
# -lreadline : flag to link readline function
SRC= main.c utils_lib/*.c
CC= cc
NAME=exe


all: $(NAME)

$(NAME):
	@ $(CC) $(SRC) $(FLAFS)  -o $(NAME) 


clean:
	@rm -f $(NAME)

re: clean all