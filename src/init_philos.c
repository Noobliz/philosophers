/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:21:17 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/28 13:23:05 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	alloc_philos_threads(char **argv, t_philo **philos, pthread_t **threads)
{
	int	amount_philos;

	amount_philos = safe_atoi(argv[1]);
	*philos = malloc(sizeof(t_philo) * amount_philos);
	*threads = malloc(sizeof(pthread_t) * amount_philos);
	if (!*philos || !*threads)
	{
		print_error("malloc error", "threads or philos");
		if (*philos)
			free(*philos);
		else
			free(*threads);
		return (-1);
	}
	return (1);
}

void	start_time_meal(t_data *data)
{
	int	i;

	i = 0;
	data->start_time = get_current_time();
	while (i < data->philos_amount)
	{
		data->philos[i].last_meal = data->start_time;
		data->philos[i].start_time = data->start_time;
		i++;
	}
}

int	create_threads(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	start_time_meal(data);
	while (i < data->philos->nb_of_philos)
	{
		if (pthread_create(&data->threads[i], NULL,
				&routine, &data->philos[i]) != 0)
		{
			print_error("pthread_create error", "threads");
			while (j < i)
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

int	init_philos(char **argv, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos_amount)
	{
		data->philos[i].id = i + 1;
		data->philos[i].nb_of_philos = data->philos_amount;
		data->philos[i].time_to_die = safe_atoi(argv[2]);
		data->philos[i].time_to_eat = safe_atoi(argv[3]);
		data->philos[i].time_to_sleep = safe_atoi(argv[4]);
		if (argv[5])
			data->philos[i].nb_times_to_eat = safe_atoi(argv[5]);
		else
			data->philos[i].nb_times_to_eat = -1;
		data->philos[i].left_fork = &data->fork[i];
		data->philos[i].right_fork = &data->fork[(i + 1) % data->philos_amount];
		data->philos[i].print_mutex = &data->print_mutex;
		data->philos[i].stop_mutex = &data->stop_mutex;
		data->philos[i].eating_mutex = &data->eating_mutex;
		data->philos[i].stop = &data->stop;
		data->philos[i].meals_flag = 0;
		i++;
	}
	return (1);
}
