/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 18:09:24 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/21 19:46:18 by dievarga         ###   ########.fr       */
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

void	ft_usleep(long long milliseconds, tbox *box)
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

	pthread_mutex_locK(&box->stop_lock);
	status = box->sim_stopped;
	pthread_mutex_unlock(&box->stop_lock);
	return (status);
}

void	print_status(t_coder *coder, char *status_msg)
{
	long long	timestamp;
	t_box		*box;

	box = (t_box *)coder->l_dongle;
	pthread_mutex_lock(&box->print_lock);
	if (!box->sim_stopped)
	{
		timestamp = get_time();
		printf("%lld %d %s/n", timestamp, coder->id, status_msg);
	}
	pthread_mutex_unlock(&box->print_lock);
}
