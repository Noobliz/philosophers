/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lguiet <lguiet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:55:03 by lguiet            #+#    #+#             */
/*   Updated: 2025/04/28 13:28:49 by lguiet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#include <limits.h>

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	print(char *str, int fd, int space)
{
	int	len;

	len = 0;
	while (str[len])
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
		return (0);
	if (str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
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
	int						i;

	i = 0;
	res = 0;
	if (str[i] == '+')
		i++;
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
		return (print_error("argc", "4 or 5 arguments needed"), 0);
	while (i < argc)
	{
		if (!is_numeric(argv[i]))
			return (print_error("numeric args"
					, "only positive numbers accepted"), 0);
		nb = safe_atoi(argv[i]);
		if (nb < 0)
			return (print_error("atoi", "overflow"), 0);
		if (nb == 0 && i == 5)
			return (print_error("nb of meals", "you can't eat 0 meals"), 0);
		if (nb == 0 && i != 1 && i != 5)
			return (print_error("simulation based on time"
					, "using instant time is not realistic"), 0);
		if (i == 1 && nb <= 0)
			return (print_error("nb of philos"
					, "there should be between 1 and 200 philosophers"), 0);
		i++;
	}
	return (1);
}
