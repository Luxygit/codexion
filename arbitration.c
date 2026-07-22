/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arbitration.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:29:20 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/22 15:28:40 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_and_lock(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->lock);
	if (dongle->queue_size > 0 && dongle->waiting_queue[0] != coder->id)
	{
		enqueue_coder(dongle, coder->id, coder->box);
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	if (dongle->in_use == 1 || is_dongle_cooling(dongle))
	{
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	dongle->in_use = 1;
	pthread_mutex_unlock(&dongle->lock);
	//print_status(coder, "has taken a dongle");
	return (1);
}

static void	drop_left(t_dongle *l_dongle)
{
	pthread_mutex_lock(&l_dongle->lock);
	l_dongle->in_use = 0;
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
	dequeue_coder(coder->l_dongle);
	pthread_mutex_unlock(&coder->l_dongle->lock);
	pthread_mutex_lock(&coder->r_dongle->lock);
	dequeue_coder(coder->r_dongle);
	pthread_mutex_unlock(&coder->r_dongle->lock);
	print_status(coder, "has taken a dongle");
	print_status(coder, "has taken a dongle");
	return (1);
}
