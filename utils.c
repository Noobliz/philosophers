
#include "philo.h"

void	free_all(t_data *data)
{
	if (data->philos)
		free(data->philos);
	data->philos = NULL;
	if (data->threads)
		free(data->threads);
	data->threads = NULL;
	if (data->fork)
		free(data->fork);
	data->fork = NULL;
	if (data->big_brother)
		free(data->big_brother);
	data->big_brother = NULL;
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

int	init_philos(char **argv, t_data *data)
{
	int	amount_philos;
	int	i;

	amount_philos = safe_atoi(argv[1]);
	i = 0;
	while (i < amount_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].nb_of_philos = amount_philos;
		data->philos[i].time_to_die = safe_atoi(argv[2]);
		data->philos[i].time_to_eat = safe_atoi(argv[3]);
		data->philos[i].time_to_sleep = safe_atoi(argv[4]);
		data->philos[i].start_time = get_current_time();
		if (argv[5])
			data->philos[i].nb_times_to_eat = safe_atoi(argv[5]);
		else
			data->philos[i].nb_times_to_eat = -1;
		data->philos[i].left_fork = &data->fork[i];
		data->philos[i].right_fork = &data->fork[(i + 1) % amount_philos];
		data->philos[i].print_mutex = &data->print_mutex;
		data->philos[i].stop_mutex = &data->stop_mutex;
		data->philos[i].eating_mutex = &data->eating_mutex;
		data->philos[i].stop = &data->stop;
		data->philos[i].meals_flag = 0;
		//data->philos[i].last_meal = get_current_time();
		i++;
	}
	if (!eating_init(data))
		return (0);
	return (1);
}

int	alloc_philos_threads(char **argv, t_philo **philos, pthread_t **threads)
{
	int amount_philos;

	amount_philos = safe_atoi(argv[1]);
	*philos = malloc(sizeof(t_philo) * amount_philos);
	*threads = malloc(sizeof(pthread_t) * amount_philos);
	if (!*philos || !*threads)
	{
		write(2, "malloc\n", 7); //PERROR FORBIDEN !!!!!!
		if (*philos)
			free(*philos);
		else
			free(*threads);
		return (-1);
	}
	return (1);
}

void safe_print(t_philo *philo, const char *msg)
{
	time_t	time;

	
	
	pthread_mutex_lock(philo->print_mutex);
	pthread_mutex_lock(philo->stop_mutex);
	time = get_current_time() - philo->start_time;
	// printf("Philo[%d] %s\n", philo->id, msg);
	if(*(philo->stop) == 0 || strcmp(msg, "is dead !!!") == 0)
	printf("[%ld]Philo[%d] %s\n", time, philo->id, msg);
	pthread_mutex_unlock(philo->print_mutex);
	pthread_mutex_unlock(philo->stop_mutex);
}