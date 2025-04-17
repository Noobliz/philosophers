
#ifndef	PHILO_H
# define PHILO_H
# ifndef MAX_PHILO
#  define MAX_PHILO 200
# endif

#include <stdio.h>
#include <unistd.h>
#include <string.h>

typedef struct	s_philo
{
	int	id;
	int	nb_of_philos;
	size_t	time_to_die;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	int	nb_times_to_eat;
}	t_philo;

void	print(char *str, int fd, int space);
int		valid_input(int argc, char **argv);
int		safe_atoi(char *str);


#endif

