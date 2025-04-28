/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutex_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:19:19 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/28 13:21:00 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	eating_init(t_data *data)
{
	if (pthread_mutex_init(&data->eating_mutex, NULL) != 0)
	{
		print_error("mutex init failed", "eating_mutex");
		free_all(data);
		return (0);
	}
	return (1);
}

static int	fork_init(t_data *data)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (i < data->philos_amount)
	{
		if (pthread_mutex_init(&data->fork[i], NULL) != 0)
		{
			print_error("mutex init failed", "fork_mutex");
			while (j < i)
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

static int	print_stop_init(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		print_error("mutex init failed", "print_mutex");
		free_all(data);
		return (0);
	}
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
	{
		print_error("mutex init failed", "stop_mutex");
		free_all(data);
		return (0);
	}
	return (1);
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
		print_error("malloc error", "forks");
		free_all(data);
		return (-1);
	}
	if (!fork_init(data))
		return (0);
	if (!print_stop_init(data))
		return (0);
	if (!eating_init(data))
		return (0);
	return (1);
}

void	join_destroy(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos->nb_of_philos)
	{
		if (data->threads)
			pthread_join(data->threads[i], NULL);
		i++;
	}
	if (data->big_brother)
		pthread_join(data->big_brother, NULL);
	i = 0;
	while (i < data->philos->nb_of_philos)
	{
		pthread_mutex_destroy(&data->fork[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
	pthread_mutex_destroy(&data->eating_mutex);
}
