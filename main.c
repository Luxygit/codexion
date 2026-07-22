/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 10:11:23 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/22 13:28:51 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	free_all(t_box *box)
{
	int		i;

	i = 0;
	while (i < box->rules.num_coders)
	{
		pthread_mutex_destroy(&box->dongles[i].lock);
		i++;
	}
	pthread_mutex_destroy(&box->print_lock);
	pthread_mutex_destroy(&box->stop_lock);
	free(box->dongles);
	free(box->coders);
	free(box->threads);
}

int	main(int ac, char **av)
{
	t_box	box;

	if (!parse_args(&box, ac, av))
		return (1);
	box.threads = malloc(sizeof(pthread_t) * box.rules.num_coders);
	box.coders = malloc(sizeof(t_coder) * box.rules.num_coders);
	box.dongles = malloc(sizeof(t_dongle) * box.rules.num_coders);
	if (!box.coders || !box.dongles || !box.threads)
	{
		free(box.coders);
		free(box.dongles);
		free(box.threads);
		write(2, "Error: Malloc failed\n", 21);
		return (1);
	}
	init_data(&box);
	start_sim(&box);
	printf("All parsed and initialised\n");
	free_all(&box);
	return (0);
}
