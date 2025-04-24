/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:55:03 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/24 14:55:06 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>

void	print(char *str, int fd, int space)
{
	int	len;
	
	len = 0;
	while(str[len])
		len++;
	write(fd, str, len);
	if (space)
		write (fd, " ", 1);
}

static int	is_numeric(char *str)
{
	int	i;
	
	i = 0;
	if (!str || !str[i])
		return(0);
	
	while(str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
int	safe_atoi(char *str)
{
	unsigned long long int	res;
	int	i;

	i = 0;
	res = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	if (res > INT_MAX)
		return (-1);
	return (res);
}

int	valid_input(int argc, char **argv)
{
	int	i;
	int	nb;

	i = 1;
	if (!(argc == 6 || argc == 5))
		return (print("4 or 5 inputs needed\n", 2, 0), 0);
	while (i < argc)
	{
		if (!is_numeric(argv[i]))
			return (print("only positive digits are accepted\n", 2, 0), 0);
		nb = safe_atoi(argv[i]);
		if (nb < 0)
			return (print("overflow\n", 2, 0), 0);
		if (nb == 0 && i != 5)
			return (print("input needs to be more than 0\n", 2, 0), 0);
		if (i == 1 && (nb <= 0 || nb > 200))
			return (print("nb of philos should be between 1 and 200\n", 2, 0), 0);
		i++;
	}
	return (1);
}
