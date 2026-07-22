/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 10:55:31 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/22 14:59:49 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	sort_queue_edf(t_dongle *dongle, t_box *box)
{
	int			id_a;
	int			id_b;
	long long	deadline_a;
	long long	deadline_b;
	int			tmp;

	if (dongle->queue_size < 2)
		return ;
	id_a = dongle->waiting_queue[0];
	id_b = dongle->waiting_queue[1];
	if (id_a <= 0 || id_b <= 0 || id_a > box->rules.num_coders
		|| id_b > box->rules.num_coders)
		return ;
	deadline_a = box->coders[id_a - 1].last_compile_time
		+ box->rules.time_to_burnout;
	deadline_b = box->coders[id_b - 1].last_compile_time
		+ box->rules.time_to_burnout;
	if (deadline_b < deadline_a)
	{
		tmp = dongle->waiting_queue[0];
		dongle->waiting_queue[0] = dongle->waiting_queue[1];
		dongle->waiting_queue[1] = tmp;
	}
}

void	enqueue_coder(t_dongle *dongle, int coder_id, t_box *box)
{
	int	i;

	i = 0;
	while (i < dongle->queue_size)
	{
		if (dongle->waiting_queue[i] == coder_id)
			return ;
		i++;
	}
	if (dongle->queue_size >= 2)
		return ;
	dongle->waiting_queue[dongle->queue_size] = coder_id;
	dongle->queue_size++;
	if (box->rules.is_edf)
		sort_queue_edf(dongle, box);
}

void	dequeue_coder(t_dongle *dongle)
{
	if (dongle->queue_size == 0)
		return ;
	dongle->waiting_queue[0] = dongle->waiting_queue[1];
	dongle->waiting_queue[1] = 0;
	dongle->queue_size--;
}
