/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 10:11:23 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/22 14:43:49 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_data(t_box *box)
{
	int		i;

	i = 0;
	while (i < box->rules.num_coders)
	{
		pthread_mutex_init(&box->dongles[i].lock, NULL);
		box->coders[i].id = i + 1;
		box->coders[i].comp_count = 0;
		box->coders[i].rules = &box->rules;
		box->coders[i].box = box;
		box->coders[i].r_dongle = &box->dongles[(i + 1)
			% box->rules.num_coders];
		box->coders[i].l_dongle = &box->dongles[i];
		box->dongles[i].in_use = 0;
		box->dongles[i].cooldown = 0;
		box->dongles[i].waiting_queue[0] = 0;
		box->dongles[i].waiting_queue[1] = 0;
		box->dongles[i].queue_size = 0;
		i++;
	}
	box->sim_stopped = 0;
	pthread_mutex_init(&box->print_lock, NULL);
	pthread_mutex_init(&box->stop_lock, NULL);
	return (1);
}

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
	free_all(&box);
	return (0);
}
