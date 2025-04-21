
#include "philo.h"


int	create_threads(t_philo *philos, pthread_t *threads, pthread_mutex_t *fork)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while(i < philos->nb_of_philos)
	{
		if(pthread_create(&threads[i], NULL, &routine, &philos[i]) != 0)
		{
			perror("threads");
			while(j < i)
			{
				pthread_join(threads[j], NULL);
				j++;
			}
			free_all(philos, threads, fork);
			return (0);
		}
		i++;
	}
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
		perror("malloc");
		if (*philos)
			free(*philos);
		else
			free(*threads);
		return (-1);
	}
	return (1);
}

int	fork_init(t_philo *philos, pthread_t *threads, pthread_mutex_t *fork)
{
	int	i;
	int	j;
	
	j = 0;
	i = 0;
	while (i < philos->nb_of_philos)
	{
		if (pthread_mutex_init(&fork[i], NULL) != 0)
		{
			perror("mutex init");
			while(j < i)
			{
				pthread_mutex_destroy(&fork[j]);
				j++;
			}
			free_all(philos, threads, fork);
			return (0);
		}
		i++;
	}
	return (1);
}
void	join_destroy(t_philo *philos, pthread_t *threads, pthread_mutex_t *fork)
{
	int	i;

	i = 0;
	while (i < philos->nb_of_philos)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	i = 0;
	while(i < philos->nb_of_philos)
	{
		pthread_mutex_destroy(&fork[i]);
		i++;
	}
}
void	*routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;
	pthread_mutex_lock(philo->left_fork);
	printf("Philo[%d] has taken the left fork 🥢\n", philo->id);
	pthread_mutex_unlock(philo->left_fork);

	pthread_mutex_lock(philo->right_fork);
	printf("Philo[%d] has taken the right fork 🥢\n", philo->id);
	pthread_mutex_unlock(philo->right_fork);
	return NULL;
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
int	init_data(t_data *data, char **argv)
{
	int	amount_philos = safe_atoi(argv[1]);
	//data->philos = philos;
	if (!alloc_philos_threads(argv, &data->philos, &data->threads))
		return (-1);
	data->fork = malloc(sizeof(pthread_mutex_t) * amount_philos);
	if (!data->fork)
	{
		free_all(data->philos, data->threads, data->fork);
		return (-1);
	}
	if (!print_init(data))
		return (0);

	return (1);
}
int	main(int argc, char **argv)
{

	t_data	data;

	data.philos = NULL;
	data.threads = NULL;
	if (!(valid_input(argc, argv)))
		return (1);
	init_data(&data, argv);
	init_philos(argv, data.philos, data.fork);
	if (!fork_init(data.philos, data.threads, data.fork))
		return (1);
	if (!create_threads(data.philos, data.threads, data.fork))
		return (1);
	join_destroy(data.philos, data.threads, data.fork);
	//print_philos(philos, philos->nb_of_philos);
	free_all(data.philos, data.threads, data.fork);
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
