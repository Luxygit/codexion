/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arbitration.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:29:20 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/23 19:37:06 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_and_lock(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->lock);
	push_heap(dongle, coder->last_compile_time, coder->id);
	while (dongle->in_use || (dongle->heap->coder_id == coder->id))
	{
		if (check_sim_status(coder->box))
		{
			pop_heap(dongle);
			pthread_mutex_unlock(&dongle->lock);
			return (0);
		}
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	}
	dongle->in_use = 1;
	pthread_mutex_unlock(&dongle->lock);
	return (1);
}

static void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->in_use = 0;
	pop_heap(dongle);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}

int	take_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->l_dongle < coder->r_dongle)
	{
		first = coder->l_dongle;
		second = coder->r_dongle;
	}
	else
	{
		first = coder->r_dongle;
		second = coder->l_dongle;
	}
	if (!check_and_lock(first, coder))
		return (0);
	if (!check_and_lock(second, coder))
	{
		release_dongle(first);
		return (0);
	}
	coder_take_dongle(coder, coder->l_dongle);
	coder_take_dongle(coder, coder->r_dongle);
	return (1);
}
