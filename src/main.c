/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:54:32 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/28 13:23:21 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
	if (!create_threads(&data))
		return (1);
	if (pthread_create(&data.big_brother, NULL, &is_watching, &data) != 0)
		return (print_error("pthread_create error", "big brother"), 1);
	join_destroy(&data);
	free_all(&data);
	return (0);
}
