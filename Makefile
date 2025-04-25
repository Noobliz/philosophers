CC=cc
FLAGS=-Wall -Werror -Wextra -g -pthread
SRC=src/main.c\
src/str_utils.c \
src/utils.c \
src/big_brother.c \
src/init_mutex_data.c \
src/init_philos.c \
src/routine.c

NAME=philo

all: $(NAME)
$(NAME): $(SRC) includes/philo.h
	$(CC) $(FLAGS) -o $(NAME) $(SRC)

clean:
	rm -f $(NAME)

fclean: clean

re: fclean all

.PHONY: clean all re fclean