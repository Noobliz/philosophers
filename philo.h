

#ifndef	PHILO_H
# define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct	s_philo
{
	int	id;
	int	nb_of_philos;
	size_t	time_to_die;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	int	nb_times_to_eat;

	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*print_mutex;
}	t_philo;

typedef struct	s_data
{
	t_philo	*philos;
	pthread_t *threads;
	pthread_mutex_t	*fork; //tab de fork
	pthread_mutex_t print_mutex;
}	t_data;


//str_utils
void	print(char *str, int fd, int space);
int		valid_input(int argc, char **argv);
int		safe_atoi(char *str);

//utils
void	free_all(t_data *data);
void	print_philos(t_philo *philos, int amount);
void	init_philos(char **argv, t_data *data);
int		alloc_philos_threads(char **argv, t_philo **philos, pthread_t **threads);

void	*routine(void *arg);
#endif

