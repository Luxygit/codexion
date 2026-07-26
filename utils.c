/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 18:09:24 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/26 06:18:26 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	return (((long long)tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long long milliseconds, t_box *box)
{
	long long	start_time;

	start_time = get_time();
	while ((get_time() - start_time) < milliseconds)
	{
		pthread_mutex_lock(&box->stop_lock);
		if (box->sim_stopped)
		{
			pthread_mutex_unlock(&box->stop_lock);
			break ;
		}
		pthread_mutex_unlock(&box->stop_lock);
		usleep(100);
	}
}

int	check_sim_status(t_box *box)
{
	int	status;

	pthread_mutex_lock(&box->stop_lock);
	status = box->sim_stopped;
	pthread_mutex_unlock(&box->stop_lock);
	return (status);
}

void	print_status(t_coder *coder, char *status_msg)
{
	long long	relative_time;
	t_box		*box;

	box = coder->box;
	pthread_mutex_lock(&box->print_lock);
	pthread_mutex_lock(&box->stop_lock);
	if (!box->sim_stopped)
	{
		if ((strcmp(status_msg, "is debugging") == 0 || strcmp(status_msg,
					"is refactoring") == 0)
			&& coder->comp_count >= box->rules.num_compiles_required)
		{
			pthread_mutex_unlock(&box->stop_lock);
			pthread_mutex_unlock(&box->print_lock);
			return ;
		}
		relative_time = get_time() - box->start_time;
		printf("%lld %d %s\n", relative_time, coder->id, status_msg);
	}
	pthread_mutex_unlock(&box->stop_lock);
	pthread_mutex_unlock(&box->print_lock);
}

int	all_coders_finished(t_box *box)
{
	int	i;
	int	done;

	i = 0;
	while (i < box->rules.num_coders)
	{
		pthread_mutex_lock(&box->stop_lock);
		done = (box->coders[i].comp_count >= box->rules.num_compiles_required);
		pthread_mutex_unlock(&box->stop_lock);
		if (!done)
			return (0);
		i++;
	}
	pthread_mutex_lock(&box->stop_lock);
	box->sim_stopped = 1;
	pthread_mutex_unlock(&box->stop_lock);
	wake_all_dongles(box);
	return (1);
}
