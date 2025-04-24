
#include "philo.h"

void	ft_sleep(size_t duration_ms)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < duration_ms)
		usleep(50);
}
size_t    get_current_time(void)
{
    struct timeval    time;
    //time_t            current_time;

    if (gettimeofday(&time, NULL) == -1)
        write(2, "error in gettimeofday process\n", 31);
    //current_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	create_threads(t_data *data)
{
	int	i;
	int	j;

	data->start_time = get_current_time();
	i = 0;
	while (i < data->philos_amount)
	{
		data->philos[i].last_meal = data->start_time;
		data->philos[i].start_time = data->start_time;
		i++;
	}
	i = 0;
	j = 0;
	while(i < data->philos->nb_of_philos)
	{
		if(pthread_create(&data->threads[i], NULL, &routine, &data->philos[i]) != 0)
		{
			perror("threads");
			while(j < i)
			{
				pthread_join(data->threads[j], NULL);
				j++;
			}
			free_all(data);
			return (0);
		}
		i++;
	}
	
	return (1);
}
int	eating_init(t_data *data)
{
	// int	i;
	// int	j;

	// i = 0;
	// j = 0;
	// while (i < data->philos_amount)
	// {
	// 	if (pthread_mutex_init(&data->philos[i].eating_mutex, NULL) != 0)
	// 	{
	// 		write(2, "mutex init\n", 10);
	// 		while(j < i)
	// 		{
	// 			pthread_mutex_destroy(&data->philos[j].eating_mutex);
	// 			j++;
	// 		}
	// 		free_all(data);
	// 		return (0);
	// 	}
	// 	i++;
	// }
	if (pthread_mutex_init(&data->eating_mutex, NULL) != 0)
	{
		write(2, "mutex init\n", 10);
		free_all(data);
		return (0);
	}
	return (1);
}
int	fork_init(t_data *data)
{
	int	i;
	int	j;
	
	j = 0;
	i = 0;
	while (i < data->philos->nb_of_philos)
	{
		if (pthread_mutex_init(&data->fork[i], NULL) != 0)
		{
			perror("mutex init");
			while(j < i)
			{
				pthread_mutex_destroy(&data->fork[j]);
				j++;
			}
			free_all(data);
			return (0);
		}
		i++;
	}
	return (1);
}

void	join_destroy(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos->nb_of_philos)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
	pthread_join(data->big_brother, NULL);
	i = 0;
	while(i < data->philos->nb_of_philos)
	{
		pthread_mutex_destroy(&data->fork[i]);
		//pthread_mutex_destroy(&data->philos[i].eating_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
	pthread_mutex_destroy(&data->eating_mutex);
}

int	print_init(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		write(2, "print_mutex init\n", 17);
		free_all(data);
		return (0);
	}
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
	{
		write(2, "stop_mutex init\n", 16);
		free_all(data);
		return (0);
	}
	return (1);
}

void	*routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	int i = 0;
	int	meals_eaten;
	
	meals_eaten = 0;
	while (1)
	{
		if (philo->id % 2 == 0)
			usleep(1000);
		pthread_mutex_lock(philo->left_fork);
		safe_print(philo, "has taken the left fork 🥢");
		pthread_mutex_lock(philo->right_fork);
		safe_print(philo, "has taken the right fork 🥢");

		pthread_mutex_lock(philo->eating_mutex); // pr futur nb de repas
		philo->last_meal = get_current_time();
		safe_print(philo, "is eating 🍝");
		if (philo->nb_times_to_eat > 0)
		{
			meals_eaten++;
			// pthread_mutex_lock(philo->print_mutex);
			// printf("philo[%d] meals[%d]\n", philo->id, meals_eaten);
			// pthread_mutex_unlock(philo->print_mutex);
			if (meals_eaten == philo->nb_times_to_eat)
				philo->meals_flag = 1;

		}
		pthread_mutex_unlock(philo->eating_mutex);
		usleep(philo->time_to_eat * 1000);

		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);

		safe_print(philo, "is sleeping 😴");
		usleep(philo->time_to_sleep * 1000);

		safe_print(philo, "is thinking 🧠");
		pthread_mutex_lock(philo->stop_mutex);
		if (*(philo->stop) == 1)
		{
			pthread_mutex_unlock(philo->stop_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(philo->stop_mutex);
		i++;
	}
	return NULL;
}

int	init_data(t_data *data, char **argv)
{
	data->stop = 0;
	//data->big_brother = NULL;
	data->philos_amount = safe_atoi(argv[1]);
	if (!alloc_philos_threads(argv, &data->philos, &data->threads))
	return (-1);
	//data->big_brother = malloc(sizeof(pthread_mutex_t));
	data->fork = malloc(sizeof(pthread_mutex_t) * data->philos_amount);
	if (!data->fork)
	{
		free_all(data);
		return (-1);
	}
	if (!print_init(data))
		return (0);
	return (1);
}
int	main(int argc, char **argv)
{
	t_data	data;
	//pthread_t	*big_brother;

	data.philos = NULL;
	data.threads = NULL;
	if (!(valid_input(argc, argv)))
		return (1);
	if (!init_data(&data, argv))
		return (1);
	init_philos(argv, &data);
	if (!fork_init(&data))
		return (1);
	if (!create_threads(&data))
		return (1);
	pthread_create(&data.big_brother, NULL, &is_watching, &data);
	join_destroy(&data);
	//print_philos(philos, philos->nb_of_philos);
	free_all(&data);
	return (0);
}
// int	main(int argc, char **argv)
// {
// 	t_philo *philos;
// 	pthread_t *threads;
// 	pthread_mutex_t	*fork;
// 	t_data	data;
// 	int	amount_philos;



// (:



// 	philos = NULL;
// 	threads = NULL;
// 	if (!(valid_input(argc, argv)) || !(alloc_philos_threads(argv, &philos, &threads)))
// 		return (1);
// 	amount_philos = safe_atoi(argv[1]);
// 	fork = malloc(sizeof(pthread_mutex_t) * amount_philos);
// 	if (!fork)
// 	{
// 		free_all(philos, threads, fork);
// 		return (1);
// 	}
// 	init_philos(argv, philos, fork);
// 	if (!fork_init(philos, threads, fork))
// 		return (1);
// 	if (!create_threads(philos, threads, fork))
// 		return (1);
// 	join_destroy(philos, threads, fork);
// 	//print_philos(philos, philos->nb_of_philos);
// 	free_all(philos, threads, fork);
// 	return (0);
// }
