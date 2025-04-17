CC=cc
FLAGS=-Wall -Werror -Wextra -g
SRC=main.c\
str_utils.c

NAME=philo

all=$(NAME)
$(NAME): $(SRC) philo.h
	$(CC) $(FLAGS) -o $(NAME) $(SRC)

clean:
	rm -f $(NAME)

fclean: clean

re: fclean all

.PHONY: clean all re fclean