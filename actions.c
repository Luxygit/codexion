/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:09:59 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/28 16:27:33 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	coder_compile(t_coder *coder)
{
	print_status(coder, "is compiling");
	pthread_mutex_lock(&coder->box->stop_lock);
	coder->last_compile_time = get_time();
	pthread_mutex_unlock(&coder->box->stop_lock);
	ft_usleep(coder->rules->time_to_compile, coder->box);
	pthread_mutex_lock(&coder->box->stop_lock);
	coder->comp_count++;
	pthread_mutex_unlock(&coder->box->stop_lock);
}

void	coder_debug(t_coder *coder)
{
	if (check_sim_status(coder->box) || all_coders_finished(coder->box))
		return ;
	print_status(coder, "is debugging");
	ft_usleep(coder->rules->time_to_debug, coder->box);
}

void	coder_refactor(t_coder *coder)
{
	if (!check_sim_status(coder->box) && !all_coders_finished(coder->box))
	{
		print_status(coder, "is refactoring");
		ft_usleep(coder->rules->time_to_refactor, coder->box);
	}
	if (coder->l_dongle < coder->r_dongle)
	{
		release_dongle(coder->l_dongle, coder->id);
		release_dongle(coder->r_dongle, coder->id);
	}
	else
	{
		release_dongle(coder->r_dongle, coder->id);
		release_dongle(coder->l_dongle, coder->id);
	}
}

void	coder_take_dongle(t_coder *coder, t_dongle *dongle)
{
	(void)dongle;
	print_status(coder, "has taken a dongle");
}

int	coder_has_finished(t_coder *coder)
{
	int	finished;

	pthread_mutex_lock(&coder->box->stop_lock);
	finished = (coder->comp_count >= coder->rules->num_compiles_required);
	pthread_mutex_unlock(&coder->box->stop_lock);
	return (finished);
}
