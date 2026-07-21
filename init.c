/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 14:57:07 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/21 19:56:22 by dievarga         ###   ########.fr       */
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
		box->coders[i].r_dongle = &box->dongles[(i + 1)
			% box->rules.num_coders];
		box->coders[i].l_dongle = &box->dongles[i];
		box->dongles[i].in_use = 0;
		box->dongles[i].cooldown = 0;
		box->dongles[i].waiting_queue[0] = 0;
		box->dongles[i].waiting_queue[1] = 0;
		box->dongles[i].queue_size = 0;
		box->coders[i].box = box;
		i++;
	}
	box->sim_stopped = 0;
	pthread_mutex_init(&box->print_lock, NULL);
	pthread_mutex_init(&box->stop_lock, NULL);
	return (1);
}
