

#ifndef	PHILO_H
# define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


#include <sys/time.h>



typedef struct	s_philo
{
	int	id;
	int	nb_of_philos;
	int	*stop;
	size_t	time_to_die;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	size_t	start_time;
	int	nb_times_to_eat;
	int	meals_flag;

	size_t	last_meal;
	
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*print_mutex;
	pthread_mutex_t *stop_mutex;
	pthread_mutex_t *eating_mutex;
}	t_philo;

typedef struct	s_data
{
	size_t	start_time;
	int	philos_amount;
	int	stop;
	t_philo	*philos;
	pthread_t *threads;
	pthread_t	*big_brother;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	*fork;
	pthread_mutex_t print_mutex;
	pthread_mutex_t	eating_mutex;
}	t_data;


//str_utils
void	print(char *str, int fd, int space);
int		valid_input(int argc, char **argv);
int		safe_atoi(char *str);
void 	safe_print(t_philo *philo, const char *msg);

//utils
void	free_all(t_data *data);
void	print_philos(t_philo *philos, int amount);
int		init_philos(char **argv, t_data *data);
int		alloc_philos_threads(char **argv, t_philo **philos, pthread_t **threads);

//main
int		eating_init(t_data *data);
void	*routine(void *arg);
size_t    get_current_time(void);
void	ft_sleep(size_t duration_ms);

//big_brother
void	*is_watching(void *arg);
#endif

