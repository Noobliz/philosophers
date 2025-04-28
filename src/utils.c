/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:55:11 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/28 14:54:12 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
}

void	safe_print(t_philo *philo, const char *msg)
{
	time_t	time;

	pthread_mutex_lock(philo->stop_mutex);
	pthread_mutex_lock(philo->print_mutex);
	time = get_current_time() - philo->start_time;
	if (*(philo->stop) == 0 || ft_strcmp(msg, "died 💀") == 0)
		printf("[%ld]Philo_%d %s\n", time, philo->id, msg);
	pthread_mutex_unlock(philo->print_mutex);
	pthread_mutex_unlock(philo->stop_mutex);
}

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
			break ;
		}
		pthread_mutex_unlock(philos->stop_mutex);
		usleep(1);
	}
}

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		print_error("gettimeofday error", NULL);
		return ((size_t) - 1);
	}
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
