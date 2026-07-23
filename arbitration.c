/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arbitration.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:29:20 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/23 17:02:18 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long long	get_priority(t_coder *coder)
{
	long long	priority;

	if (coder->box->rules.is_edf)
	{
		priority = coder->last_compile_time
			+ coder->box->rules.time_to_burnout;
	}
	else
	{
		pthread_mutex_lock(&coder->box->stop_lock);
		priority = coder->box->ticket_counter++;
		pthread_mutex_unlock(&coder->box->stop_lock);
	}
	return (priority);
}

static void	wait_cooldown(t_dongle *dongle, t_box *box)
{
	while (get_time() < dongle->cooldown && !check_sim_status(box))
	{
		pthread_mutex_unlock(&dongle->lock);
		usleep(200);
		pthread_mutex_lock(&dongle->lock);
	}
}

static int	check_and_lock(t_dongle *dongle, t_coder *coder)
{
	long long	priority;

	pthread_mutex_lock(&dongle->lock);
	wait_cooldown(dongle, coder->box);
	priority = get_priority(coder);
	push_heap(dongle, coder->id, priority);
	while (!check_sim_status(coder->box) && ((dongle->heap_size > 0
				&& dongle->heap[0].coder_id != coder->id)
			|| dongle->in_use == 1))
	{
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	}
	if (check_sim_status(coder->box))
	{
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	dongle->in_use = 1;
	pthread_mutex_unlock(&dongle->lock);
	return (1);
}

static void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->in_use = 0;
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
		pthread_mutex_lock(&first->lock);
		pthread_mutex_unlock(&first->lock);
		return (0);
	}
	coder_take_dongle(coder, coder->l_dongle);
	coder_take_dongle(coder, coder->r_dongle);
	return (1);
}
