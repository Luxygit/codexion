/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arbitration.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:29:20 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/23 02:11:24 by dievarga         ###   ########.fr       */
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

static void	wait_cooldown(t_dongle *dongle)
{
	long long	rem;

	while (get_time() < dongle->cooldown)
	{
		rem = dongle->cooldown - get_time();
		pthread_mutex_unlock(&dongle->lock);
		if (rem > 0)
			usleep(rem * 1000);
		pthread_mutex_lock(&dongle->lock);
	}
}

static int	check_and_lock(t_dongle *dongle, t_coder *coder)
{
	long long	priority;

	pthread_mutex_lock(&dongle->lock);
	wait_cooldown(dongle);
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

static void	drop_left(t_dongle *l_dongle)
{
	pthread_mutex_lock(&l_dongle->lock);
	l_dongle->in_use = 0;
	pthread_cond_broadcast(&l_dongle->cond);
	pthread_mutex_unlock(&l_dongle->lock);
}

int	take_both_dongles(t_coder *coder)
{
	if (!check_and_lock(coder->l_dongle, coder))
		return (0);
	if (!check_and_lock(coder->r_dongle, coder))
	{
		drop_left(coder->l_dongle);
		return (0);
	}
	pthread_mutex_lock(&coder->l_dongle->lock);
	pop_heap(coder->l_dongle);
	pthread_cond_broadcast(&coder->l_dongle->cond);
	pthread_mutex_unlock(&coder->l_dongle->lock);
	pthread_mutex_lock(&coder->r_dongle->lock);
	pop_heap(coder->r_dongle);
	pthread_cond_broadcast(&coder->r_dongle->cond);
	pthread_mutex_unlock(&coder->r_dongle->lock);
	print_status(coder, "has taken a dongle");
	print_status(coder, "has taken a dongle");
	return (1);
}
