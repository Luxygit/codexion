/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 13:27:39 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/22 13:16:42 by dievarga         ###   ########.fr       */
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

static int	validate_args(int ac, char **av)
{
	int		i;

	if (ac != 9)
		return (write(2, "Error: Wrong number of args\n", 28), 0);
	i = 1;
	while (i < 8)
	{
		if (ft_is_num(av[i]) == 0 || ft_atoi_safe(av[i]) <= 0)
			return (printf("Error: Invalid arg %s\n", av[i]), 0);
		i++;
	}
	if (strcmp(av[8], "fifo") != 0 && strcmp(av[8], "edf") != 0)
		return (write(2, "Error, last arg must be fifo or edf\n", 36), 0);
	return (1);
}

int	parse_args(t_box *box, int ac, char **av)
{
	if (!validate_args(ac, av))
		return (0);
	box->rules.num_coders = ft_atoi_safe(av[1]);
	box->rules.time_to_burnout = ft_atoi_safe(av[2]);
	box->rules.time_to_compile = ft_atoi_safe(av[3]);
	box->rules.time_to_debug = ft_atoi_safe(av[4]);
	box->rules.time_to_refactor = ft_atoi_safe(av[5]);
	box->rules.num_compiles_required = ft_atoi_safe(av[6]);
	box->rules.dongle_cooldown = ft_atoi_safe(av[7]);
	if (strcmp(av[8], "edf") == 0)
		box->rules.is_edf = 1;
	return (1);
}
