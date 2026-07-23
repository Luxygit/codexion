/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 13:27:39 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/23 17:22:43 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	ft_atoi_safe(char *str)
{
	long long	res;
	int			i;

	res = 0;
	i = 0;
	while (str[i])
	{
		res = (res * 10) + (str[i] - '0');
		if (res > 2147483647)
			return (-1);
		i++;
	}
	return (res);
}

int	ft_is_num(char *str)
{
	int		i;

	i = 0;
	if (!str || str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	validate_ranges(long long val, int index)
{
	if (index == 1 && (val < 2 || val > 200))
	{
		write(2, "Error: Coder count must be between 2 and 200\n", 45);
		return (0);
	}
	if (index == 2 && (val < 100 || val > 100000))
	{
		write(2, "Error: Burnout limit must be 100ms - 100s\n", 41);
		return (0);
	}
	if (index == 6 && (val < 1 || val > 1000))
	{
		write(2, "Error: Compile limits must be 1 - 1000\n", 39);
		return (0);
	}
	return (1);
}

static int	check_num_bounds(char **av)
{
	int			i;
	long long	val;

	i = 1;
	while (i < 8)
	{
		if (ft_is_num(av[i]) == 0)
		{
			write(2, "Error: Arguments must be numeric\n", 33);
			return (0);
		}
		val = ft_atoi_safe(av[i]);
		if (val < 0 || (val == 0 && i != 3 && i != 4 && i != 5 && i != 7)
			|| (i >= 3 && i <= 5 && val > 10000) || (i == 7 && val > 5000))
		{
			write(2, "Error: Invalid argument values or ranges\n", 41);
			return (0);
		}
		if (!validate_ranges(val, i))
			return (0);
		i++;
	}
	return (1);
}

int	validate_args(int ac, char **av)
{
	if (ac != 9)
	{
		write(2, "Error: Wrong number of args\n", 28);
		return (0);
	}
	if (!check_num_bounds(av))
		return (0);
	if (strcmp(av[8], "fifo") != 0 && strcmp(av[8], "edf") != 0)
	{
		write(2, "Error: Scheduler must be fifo or edf\n", 37);
		return (0);
	}
	if (ft_atoi_safe(av[2]) <= (ft_atoi_safe(av[3]) + ft_atoi_safe(av[4])
			+ ft_atoi_safe(av[5]) + ft_atoi_safe(av[7])))
	{
		write(2, "Error: Imposible Simulation (lifetime too short)\n", 49);
		return (0);
	}
	return (1);
}
