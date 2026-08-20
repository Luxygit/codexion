/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 11:09:59 by dievarga          #+#    #+#             */
/*   Updated: 2026/08/10 02:51:45 by dievarga         ###   ########.fr       */
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
	if (coder->l_dongle < coder->r_dongle)
	{
		release_dongle(coder->l_dongle);
		release_dongle(coder->r_dongle);
	}
	else
	{
		release_dongle(coder->r_dongle);
		release_dongle(coder->l_dongle);
	}
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
	if (check_sim_status(coder->box))
		return ;
	print_status(coder, "is refactoring");
	ft_usleep(coder->rules->time_to_refactor, coder->box);
}

void	print_both_dongles(t_coder *coder)
{
	long long	relative_time;
	t_box		*box;

	box = coder->box;
	pthread_mutex_lock(&box->print_lock);
	pthread_mutex_lock(&box->stop_lock);
	if (!box->sim_stopped)
	{
		relative_time = get_time() - box->start_time;
		printf("%lld %d has taken a dongle\n", relative_time, coder->id);
		printf("%lld %d has taken a dongle\n", relative_time, coder->id);
	}
	coder->request_prio = -1;
	pthread_mutex_unlock(&box->stop_lock);
	pthread_mutex_unlock(&box->print_lock);
}

int	coder_has_finished(t_coder *coder)
{
	int	finished;

	pthread_mutex_lock(&coder->box->stop_lock);
	finished = (coder->comp_count >= coder->rules->num_compiles_required);
	pthread_mutex_unlock(&coder->box->stop_lock);
	return (finished);
}
