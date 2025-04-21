
#include "philo.h"

void	free_all(t_philo *philos, pthread_t *threads, pthread_mutex_t *fork)
{
	if (philos)
		free(philos);
	philos = NULL;
	if (threads)
		free(threads);
	threads = NULL;
	if (fork)
		free(fork);
	fork = NULL;
}

void	print_philos(t_philo *philos, int amount)
{
	int	i = 0;

	while(i < amount)
	{
		printf("-------------------\n");
		printf("philo n[%d]\n", philos[i].id);
		printf("philo time to die[%ld]\n", philos[i].time_to_die);
		printf("philo time to eat[%ld]\n", philos[i].time_to_eat);
		printf("philo time to sleep[%ld]\n", philos[i].time_to_sleep);
		printf("philo nb of times to eat[%d]\n", philos[i].nb_times_to_eat);
		printf("-------------------\n");
		i++;
	}
}

void	init_philos(char **argv, t_philo *philos, pthread_mutex_t *fork)
{
	int	amount_philos;
	int	i;

	amount_philos = safe_atoi(argv[1]);
	i = 0;
	while (i < amount_philos)
	{
		philos[i].id = i + 1;
		philos[i].nb_of_philos = amount_philos;
		philos[i].time_to_die = safe_atoi(argv[2]);
		philos[i].time_to_eat = safe_atoi(argv[3]);
		philos[i].time_to_sleep = safe_atoi(argv[4]);
		if (argv[5])
			philos[i].nb_times_to_eat = safe_atoi(argv[5]);
		else
			philos[i].nb_times_to_eat = -1;
		philos[i].left_fork = &fork[i];
		philos[i].right_fork = &fork[(i + 1) % amount_philos];
		i++;
	}
}