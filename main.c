/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 10:11:23 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/23 02:43:27 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	allocate_heaps(t_box *box)
{
	int	i;

	i = 0;
	while (i < box->rules.num_coders)
	{
		box->dongles[i].heap = malloc(sizeof(t_heap_node)
				* box->rules.num_coders);
		if (!box->dongles[i].heap)
			return (0);
		box->dongles[i].heap_size = 0;
		box->dongles[i].capacity = box->rules.num_coders;
		i++;
	}
	return (1);
}

int	init_data(t_box *box)
{
	int		i;

	i = -1;
	box->sim_stopped = 0;
	box->ticket_counter = 0;
	while (++i < box->rules.num_coders)
	{
		pthread_mutex_init(&box->dongles[i].lock, NULL);
		pthread_cond_init(&box->dongles[i].cond, NULL);
		box->coders[i].id = i + 1;
		box->coders[i].comp_count = 0;
		box->coders[i].rules = &box->rules;
		box->coders[i].box = box;
		box->coders[i].r_dongle = &box->dongles[(i + 1)
			% box->rules.num_coders];
		box->coders[i].l_dongle = &box->dongles[i];
		box->dongles[i].in_use = 0;
		box->dongles[i].cooldown = 0;
	}
	pthread_mutex_init(&box->print_lock, NULL);
	pthread_mutex_init(&box->stop_lock, NULL);
	return (allocate_heaps(box));
}

void	free_all(t_box *box)
{
	int		i;

	i = 0;
	if (box->dongles)
	{
		while (i < box->rules.num_coders)
		{
			pthread_cond_broadcast(&box->dongles[i].cond);
			pthread_mutex_destroy(&box->dongles[i].lock);
			pthread_cond_destroy(&box->dongles[i].cond);
			free(box->dongles[i].heap);
			i++;
		}
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

	memset(&box, 0, sizeof(t_box));
	if (!parse_args(&box, ac, av))
		return (1);
	box.threads = malloc(sizeof(pthread_t) * box.rules.num_coders);
	box.coders = malloc(sizeof(t_coder) * box.rules.num_coders);
	box.dongles = malloc(sizeof(t_dongle) * box.rules.num_coders);
	if (!box.coders || !box.dongles || !box.threads || !init_data(&box))
	{
		free_all(&box);
		write(2, "Error: Setup failed\n", 21);
		return (1);
	}
	start_sim(&box);
	free_all(&box);
	return (0);
}
