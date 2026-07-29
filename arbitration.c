/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arbitration.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:29:20 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/29 14:46:51 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long long	get_priority(t_coder *coder)
{
	long long	prio;

	pthread_mutex_lock(&coder->box->stop_lock);
	if (coder->box->rules.is_edf)
		prio = coder->last_compile_time + coder->box->rules.time_to_burnout;
	else if (coder->request_prio >= 0)
		prio = coder->request_prio;
	else
	{
		prio = coder->box->ticket_counter;
		coder->box->ticket_counter++;
		coder->request_prio = prio;
	}
	pthread_mutex_unlock(&coder->box->stop_lock);
	return (prio);
}

static int	dongle_busy(t_dongle *dongle, int coder_id)
{
	if (dongle->in_use)
		return (1);
	if (get_time() < dongle->available_at)
		return (1);
	if (dongle->heap_size > 0 && dongle->heap[0].coder_id != coder_id)
		return (1);
	return (0);
}

static int	check_and_lock(t_dongle *dongle, t_coder *coder, long long prio,
		int can_wait)
{
	pthread_mutex_lock(&dongle->lock);
	push_heap(dongle, coder->id, prio);
	while (can_wait && !check_sim_status(coder->box)
		&& dongle_busy(dongle, coder->id))
	{
		if (get_time() < dongle->available_at)
		{
			pthread_mutex_unlock(&dongle->lock);
			usleep(50);
			pthread_mutex_lock(&dongle->lock);
			continue ;
		}
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	}
	if (check_sim_status(coder->box) || dongle_busy(dongle, coder->id))
	{
		pop_heap(dongle, coder->id);
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	dongle->in_use = 1;
	pop_heap(dongle, coder->id);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
	return (1);
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->in_use = 0;
	dongle->available_at = get_time() + dongle->cooldown_duration;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}

int	take_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	long long	prio;

	if (coder->rules->num_coders == 1)
		return (0);
	prio = get_priority(coder);
	first = coder->l_dongle;
	second = coder->r_dongle;
	if (coder->l_dongle > coder->r_dongle)
	{
		first = coder->r_dongle;
		second = coder->l_dongle;
	}
	if (!check_and_lock(first, coder, prio, 1))
		return (0);
	if (!check_and_lock(second, coder, prio, 0))
	{
		release_dongle(first);
		return (0);
	}
	print_both_dongles(coder);
	return (1);
}
