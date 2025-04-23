
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
		i = 0;
		while (i < data->philos_amount)
		{
			pthread_mutex_lock(&data->eating_mutex);
			time_last_meal = get_current_time() - data->philos[i].last_meal;
			pthread_mutex_unlock(&data->eating_mutex);

			pthread_mutex_lock(&data->stop_mutex);
			if (data->stop == 1)
			{
				pthread_mutex_unlock(&data->stop_mutex);
				return (NULL);
			}
			if (time_last_meal >= data->philos[i].time_to_die)
			{
				data->stop = 1;
				pthread_mutex_unlock(&data->stop_mutex);
				safe_print(&data->philos[i], "is dead !!!");
				return (NULL);
			}
			pthread_mutex_lock(&data->eating_mutex);
			if (data->philos[i].nb_times_to_eat > 0)
			{
				if(data->philos[i].meals_flag == 1)
					count++;
				if (count == data->philos_amount)
				{
					data->stop = 1;
					pthread_mutex_unlock(&data->eating_mutex);
					pthread_mutex_unlock(&data->stop_mutex);
					return (NULL);
				}
			}
			pthread_mutex_unlock(&data->eating_mutex);
			pthread_mutex_unlock(&data->stop_mutex);
			i++;
		}
		count = 0;
		usleep(1000);
	}
	return (NULL);
}