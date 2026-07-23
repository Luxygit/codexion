/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 17:54:37 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/23 12:00:39 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!check_sim_status(coder->box))
	{
		if (coder->comp_count == coder->rules->num_compiles_required)
		{
			usleep(5000);
			continue ;
		}
		if (!take_both_dongles(coder))
			continue ;
		coder_compile(coder);
		if (!check_sim_status(coder->box))
			coder_debug(coder);
		if (!check_sim_status(coder->box))
			coder_refactor(coder);
		usleep(1000);
	}
	return (NULL);
}

int	start_sim(t_box *box)
{
	int			i;
	pthread_t	monitor;
	long long	start_time;

	i = -1;
	start_time = get_time();
	box->start_time = start_time;
	while (++i < box->rules.num_coders)
		box->coders[i].last_compile_time = start_time;
	i = 0;
	while (i < box->rules.num_coders)
	{
		pthread_create(&box->threads[i], NULL, coder_routine, &box->coders[i]);
		i++;
	}
	pthread_create(&monitor, NULL, burnout_monitor, box);
	i = 1;
	while (++i < box->rules.num_coders)
		pthread_join(box->threads[i], NULL);
	pthread_join(monitor, NULL);
	return (0);
}

void	wake_all_dongles(t_box *box)
{
	int	i;

	i = 0;
	while (i < box->rules.num_coders)
	{
		pthread_mutex_lock(&box->dongles[i].lock);
		pthread_cond_broadcast(&box->dongles[i].cond);
		pthread_mutex_unlock(&box->dongles[i].lock);
		i++;
	}
}

void	*burnout_monitor(void *arg)
{
	t_box	*box;
	int		i;

	box = (t_box *)arg;
	while (!check_sim_status(box))
	{
		if (all_coders_finished(box))
			return (NULL);
		i = -1;
		while (++i < box->rules.num_coders)
		{
			if (get_time() - box->coders[i].last_compile_time
				> box->rules.time_to_burnout)
			{
				print_status(&box->coders[i], "burned out");
				pthread_mutex_lock(&box->stop_lock);
				box->sim_stopped = 1;
				pthread_mutex_unlock(&box->stop_lock);
				wake_all_dongles(box);
				return (NULL);
			}
		}
		usleep(500);
	}
	return (NULL);
}
