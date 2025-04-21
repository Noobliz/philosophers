CC=cc
FLAGS=-Wall -Werror -Wextra -g -pthread
SRC=main.c\
str_utils.c \
utils.c

NAME=philo

all=$(NAME)
$(NAME): $(SRC) philo.h
	$(CC) $(FLAGS) -o $(NAME) $(SRC)

clean:
	rm -f $(NAME)

fclean: clean

re: fclean all

.PHONY: clean all re fclean