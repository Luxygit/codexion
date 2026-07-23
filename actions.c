/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:09:59 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/23 17:04:40 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	coder_compile(t_coder *coder)
{
	print_status(coder, "is compiling");
	coder->last_compile_time = get_time();
	ft_usleep(coder->rules->time_to_compile, coder->box);
	coder->comp_count++;
	pthread_mutex_lock(&coder->l_dongle->lock);
	coder->l_dongle->in_use = 0;
	coder->l_dongle->cooldown = get_time() + coder->rules->dongle_cooldown;
	pthread_cond_broadcast(&coder->l_dongle->cond);
	pthread_mutex_unlock(&coder->l_dongle->lock);
	pthread_mutex_lock(&coder->r_dongle->lock);
	coder->r_dongle->in_use = 0;
	coder->r_dongle->cooldown = get_time() + coder->rules->dongle_cooldown;
	pthread_cond_broadcast(&coder->r_dongle->cond);
	pthread_mutex_unlock(&coder->r_dongle->lock);
}

void	coder_debug(t_coder *coder)
{
	print_status(coder, "is debugging");
	ft_usleep(coder->rules->time_to_debug, coder->box);
}

void	coder_refactor(t_coder *coder)
{
	print_status(coder, "is refactoring");
	ft_usleep(coder->rules->time_to_refactor, coder->box);
	coder->last_compile_time = get_time();
}

void	coder_take_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	pop_heap(dongle);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
	print_status(coder, "has taken a dongle");
}
