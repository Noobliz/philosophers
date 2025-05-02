CC=cc
NAME= philo
FLAGS=-Wall -Werror -Wextra -g -pthread
SRC=src/main.c\
src/str_utils.c \
src/utils.c \
src/big_brother.c \
src/init_mutex_data.c \
src/init_philos.c \
src/routine.c

OBJ    = $(SRC:.c=.o)
INC    = includes

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -I$(INC) -o $(NAME) $(OBJ)

%.o: %.c
	@$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re