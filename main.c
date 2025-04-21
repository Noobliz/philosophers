
#include "philo.h"

void safe_print(t_philo *philo, const char *msg)
{
	pthread_mutex_lock(philo->print_mutex);
	printf("Philo[%d] %s\n", philo->id, msg);
	fflush(stdout); 
	pthread_mutex_unlock(philo->print_mutex);
}

int	create_threads(t_data *data)
{
	int	i;
	int	j;

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
	i = 0;
	while(i < data->philos->nb_of_philos)
	{
		pthread_mutex_destroy(&data->fork[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
}
int	print_init(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		perror("print_mutex init");
		return (0);
	}
	return (1);
}
// void	*routine(void *arg)
// {
// 	t_philo	*philo = (t_philo *)arg;
// 	pthread_mutex_lock(philo->left_fork);
// 	safe_print(philo, "has taken the left fork 🥢");
// 	pthread_mutex_unlock(philo->left_fork);

// 	pthread_mutex_lock(philo->right_fork);
// 	safe_print(philo, "has taken the right fork 🥢");
// 	pthread_mutex_unlock(philo->right_fork);
// 	return NULL;
// }

void	*routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	int i = 0;
	while (i < 3)
	{
		//pthread_mutex_lock(philo->left_fork);
		safe_print(philo, "has taken the left fork 🥢");
		pthread_mutex_lock(philo->right_fork);
		safe_print(philo, "has taken the right fork 🥢");

		safe_print(philo, "is eating 🍝");
		usleep(philo->time_to_eat * 1000); // conversion

		//pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);

		safe_print(philo, "is sleeping 😴");
		usleep(philo->time_to_sleep * 1000);

		safe_print(philo, "is thinking 🧠");
		i++;
	}
	return NULL;
}

int	init_data(t_data *data, char **argv)
{
	int	amount_philos = safe_atoi(argv[1]);
	//data->philos = philos;
	if (!alloc_philos_threads(argv, &data->philos, &data->threads))
		return (-1);
	data->fork = malloc(sizeof(pthread_mutex_t) * amount_philos);
	if (!data->fork)
	{
		free_all(data);
		return (-1);
	}
	if (!print_init(data))
	{
		free_all(data);
		return (0);
	}
	return (1);
}
int	main(int argc, char **argv)
{
	t_data	data;

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
