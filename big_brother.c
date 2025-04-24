/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_brother.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:54:46 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/24 14:54:49 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// routine for big brother :
//check if a philo has to die comparing last meal time and time to die
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
			if (count == data->philos_amount || (time_last_meal >= data->philos[i].time_to_die))
			{
				pthread_mutex_lock(&data->stop_mutex);
				data->stop = 1;
				pthread_mutex_unlock(&data->stop_mutex);
				if (time_last_meal >= data->philos[i].time_to_die)
					safe_print(&data->philos[i], "is dead !!!");

				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}