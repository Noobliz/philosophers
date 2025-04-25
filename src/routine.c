
#include "../includes/philo.h"

static int	one_philo(t_philo *philo)
{

	if (philo->nb_of_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		safe_print(philo, "has taken left fork 🥢");
		ft_sleep(philo->time_to_die, philo);
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	return (0);
}

static void	forks_attrib(t_philo *philo)
{
	
	if (philo->id % 2 == 0)
	{
		usleep(1);
		pthread_mutex_lock(philo->left_fork);
		safe_print(philo, "has taken a fork 🥢");
	
		pthread_mutex_lock(philo->right_fork);
		safe_print(philo, "has taken a fork 🥢");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		safe_print(philo, "has taken a fork 🥢");
		pthread_mutex_lock(philo->left_fork);
		safe_print(philo, "has taken a fork 🥢");
	
	 }
}

static void	forks_retriev(t_philo *philo)
{
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
}

static void	eating_routine(t_philo *philo, int *meals_eaten)
{
	forks_attrib(philo);
	pthread_mutex_lock(philo->eating_mutex);
	philo->last_meal = get_current_time();
	safe_print(philo, "is eating 🍝");
	if (philo->nb_times_to_eat >= 0)
	{
		(*meals_eaten)++;
		if (*(meals_eaten) == philo->nb_times_to_eat)
			philo->meals_flag = 1;
	}
	pthread_mutex_unlock(philo->eating_mutex);
	ft_sleep(philo->time_to_eat, philo);
	forks_retriev(philo);

}

void	*routine(void *arg)
{
	t_philo *philo;
	int	meals_eaten;
	
	philo = (t_philo *)arg;
	meals_eaten = 0;
	if (one_philo(philo))
		return (NULL);
	while (1)
	{
		eating_routine(philo, &meals_eaten);
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
		usleep(200);
	}
	return NULL;
}
