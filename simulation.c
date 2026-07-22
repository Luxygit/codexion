/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 17:54:37 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/22 13:33:57 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_dongle_cooling(t_dongle *dongle)
{
	long long	current_time;

	current_time = get_time();
	if (current_time < dongle->cooldown)
		return (1);
	return (0);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	coder->last_compile_time = get_time();
	while (!check_sim_status(coder->box))
	{
		if (coder->comp_count == coder->rules->num_compiles_required)
			break ;
		coder_think(coder);
		while (!check_sim_status(coder->box))
		{
			if (take_both_dongles(coder))
				break ;
			usleep(1000);
		}
		if (check_sim_status(coder->box))
			break ;
		coder_compile(coder);
		coder_debug(coder);
		coder_refactor(coder);
	}
	return (NULL);
}

int	start_sim(t_box *box)
{
	int			i;
	pthread_t	monitor;

	i = 0;
	while (i < box->rules.num_coders)
	{
		pthread_create(&box->threads[i], NULL, coder_routine, &box->coders[i]);
		i++;
	}
	pthread_create(&monitor, NULL, burnout_monitor, box);
	i = 0;
	while (i < box->rules.num_coders)
	{
		pthread_join(box->threads[i], NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	return (0);
}

int	try_get_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	if (dongle->in_use == 1 || is_dongle_cooling(dongle))
	{
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	dongle->in_use = 1;
	pthread_mutex_unlock(&dongle->lock);
	return (1);
}

void	*burnout_monitor(void *arg)
{
	t_box	*box;
	int		i;

	box = (t_box *)arg;
	while (!check_sim_status(box))
	{
		i = 0;
		while (i < box->rules.num_coders)
		{
			if (get_time() - box->coders[i].last_compile_time
				> box->rules.time_to_burnout)
			{
				pthread_mutex_lock(&box->stop_lock);
				box->sim_stopped = 1;
				pthread_mutex_unlock(&box->stop_lock);
				print_status(&box->coders[i], "burned out");
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
