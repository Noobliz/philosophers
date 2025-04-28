/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:54:55 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/28 12:44:06 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				id;
	int				nb_of_philos;
	int				nb_times_to_eat;
	int				meals_flag;
	int				*stop;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	size_t			last_meal;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*print_mutex;
	pthread_mutex_t	*stop_mutex;
	pthread_mutex_t	*eating_mutex;
}	t_philo;

typedef struct s_data
{
	int				philos_amount;
	int				stop;
	size_t			start_time;
	t_philo			*philos;
	pthread_t		*threads;
	pthread_t		big_brother;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	*fork;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	eating_mutex;
}	t_data;

//str_utils
int		ft_strcmp(const char *s1, const char *s2);
void	print(char *str, int fd, int space);
int		valid_input(int argc, char **argv);
int		safe_atoi(char *str);

//utils
void	free_all(t_data *data);
void	safe_print(t_philo *philo, const char *msg);
size_t	get_current_time(void);
void	ft_sleep(size_t duration_ms, t_philo *philos);

//routine
void	*routine(void *arg);

//big_brother
int		print_error(const char *context, const char *message);
void	*is_watching(void *arg);

//init_philos
int		alloc_philos_threads(char **argv, t_philo **philos,
			pthread_t **threads);
int		create_threads(t_data *data);
int		init_philos(char **argv, t_data *data);

//init_mutex_data
int		init_data(t_data *data, char **argv);
void	join_destroy(t_data *data);
#endif