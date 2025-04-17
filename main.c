
#include "philo.h"

void	print_philos(t_philo philos[], int amount)
{
	int	i = 0;

	while(i < amount)
	{
		printf("-------------------\n");
		printf("philo n[%d]\n", philos[i].id);
		printf("philo time to die[%ld]\n", philos[i].time_to_die);
		printf("philo time to eat[%ld]\n", philos[i].time_to_eat);
		printf("philo time to sleep[%ld]\n", philos[i].time_to_sleep);
		printf("philo nb of times to eat[%d]\n", philos[i].nb_times_to_eat);
		printf("-------------------\n");
		i++;
	}
}
void	init_philos(char **argv, t_philo philos[])
{
	int	amount_philos;
	int	i;
	amount_philos = safe_atoi(argv[1]);

	i = 0;
	while (i < amount_philos)
	{
		philos[i].id = i + 1;
		philos[i].nb_of_philos = amount_philos;
		philos[i].time_to_die = safe_atoi(argv[2]);
		philos[i].time_to_eat = safe_atoi(argv[3]);
		philos[i].time_to_sleep = safe_atoi(argv[4]);
		if (argv[5])
			philos[i].nb_times_to_eat = safe_atoi(argv[5]);
		else
			philos[i].nb_times_to_eat = -1;
		i++;
	}
}
int	main(int argc, char **argv)
{
	if (!valid_input(argc, argv))
		return (-1);
	t_philo	philos[200];
	memset(philos, 0, sizeof(philos));
	init_philos(argv, philos);
	print_philos(philos, philos->nb_of_philos);
	return (0);
}