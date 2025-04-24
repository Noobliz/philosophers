/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:54:32 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/24 17:26:39 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_sleep(size_t duration_ms, t_philo *philos)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < duration_ms)
	{
		pthread_mutex_lock(philos->stop_mutex);
		if (*(philos)->stop == 1)
		{
			pthread_mutex_unlock(philos->stop_mutex);
			break;
		}
		pthread_mutex_unlock(philos->stop_mutex);
		usleep(1);

	}
}
size_t    get_current_time(void)
{
    struct timeval    time;

    if (gettimeofday(&time, NULL) == -1)
        write(2, "error in gettimeofday process\n", 31);
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
	int	meals_eaten;

	meals_eaten = 0;
	while (1)
	{
		if (philo->nb_of_philos == 1)
		{
			pthread_mutex_lock(philo->left_fork);
			safe_print(philo, "has taken left fork 🥢");
			ft_sleep(philo->time_to_die, philo);
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_lock(philo->stop_mutex);
			if (*(philo->stop) == 1)
			{
				pthread_mutex_unlock(philo->stop_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(philo->stop_mutex);
			
		}
		if (philo->id % 2 == 0)
		{
			usleep(1000);
			pthread_mutex_lock(philo->left_fork);
			safe_print(philo, "has taken left fork 🥢");
	
			pthread_mutex_lock(philo->right_fork);
			safe_print(philo, "has taken right fork 🥢");
		}
		else
		{
			pthread_mutex_lock(philo->right_fork);
			safe_print(philo, "has taken right fork 🥢");
			pthread_mutex_lock(philo->left_fork);
			safe_print(philo, "has taken left fork 🥢");

		}
		pthread_mutex_lock(philo->eating_mutex);
		philo->last_meal = get_current_time();
		//if (*(philo->stop) == 0)
		safe_print(philo, "is eating 🍝");
		if (philo->nb_times_to_eat >= 0)
		{
			meals_eaten++;
			// pthread_mutex_lock(philo->print_mutex);
			// printf("philo[%d] meals[%d]\n", philo->id, meals_eaten);
			// pthread_mutex_unlock(philo->print_mutex);
			if (meals_eaten == philo->nb_times_to_eat)
				philo->meals_flag = 1;

		}
		pthread_mutex_unlock(philo->eating_mutex);
		ft_sleep(philo->time_to_eat, philo);
		if (philo->id % 2 == 0)
		{
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(philo->left_fork);
		}
		else
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
		}
		safe_print(philo, "is sleeping 😴");
		ft_sleep(philo->time_to_sleep, philo);
		safe_print(philo, "is thinking 🧠");
		pthread_mutex_lock(philo->stop_mutex);
		if (*(philo->stop) == 1)
		{
			pthread_mutex_unlock(philo->stop_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(philo->stop_mutex);
	}
	return NULL;
}

int	init_data(t_data *data, char **argv)
{
	data->stop = 0;
	data->philos_amount = safe_atoi(argv[1]);
	if (!alloc_philos_threads(argv, &data->philos, &data->threads))
	return (-1);
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

