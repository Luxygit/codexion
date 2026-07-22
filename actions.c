/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:09:59 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/22 13:40:28 by dievarga         ###   ########.fr       */
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
	pthread_mutex_unlock(&coder->l_dongle->lock);
	pthread_mutex_lock(&coder->r_dongle->lock);
	coder->r_dongle->in_use = 0;
	coder->r_dongle->cooldown = get_time() + coder->rules->dongle_cooldown;
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
}

void	coder_think(t_coder *coder)
{
	print_status(coder, "is thinking");
}
