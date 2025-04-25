/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_brother.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisux <lisux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:54:46 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/25 14:02:18 by lisux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static size_t	ft_strlen(const char *s)
{
	size_t	i = 0;

	while (s[i])
		i++;
	return (i);
}

int	print_error(const char *context, const char *message)
{
	if (context)
	{
		write(2, context, ft_strlen(context));
		write(2, ": ", 2);
	}
	if (message)
		write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	return (1);
}

static int	should_stop(int count, size_t time_last_meal, int i, t_data *data)
{
	if (count == data->philos_amount || (time_last_meal >= data->philos[i].time_to_die))
	{
		pthread_mutex_lock(&data->stop_mutex);
		data->stop = 1;
		pthread_mutex_unlock(&data->stop_mutex);
		if (time_last_meal >= data->philos[i].time_to_die)
			safe_print(&data->philos[i], "died 💀");
		return (1);
	}
	return (0);
}
void	*is_watching(void *arg)
{
	t_data	*data;
	size_t	time_last_meal;
	int	i;
	int	count;

	count = 0;
	data = (t_data *)arg;
	i = 0;
	while(1)
	{
		count = 0;
		i = 0;
		while (i < data->philos_amount)
		{
			pthread_mutex_lock(&data->eating_mutex);
			time_last_meal = get_current_time() - data->philos[i].last_meal;
			if (data->philos[i].nb_times_to_eat > 0 && data->philos[i].meals_flag == 1)
				count++;
			pthread_mutex_unlock(&data->eating_mutex);
			if (should_stop(count, time_last_meal, i, data))
				return (NULL);
			i++;
		}
		usleep(1);
	}
}
