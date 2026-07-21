/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 17:54:37 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/21 19:54:48 by dievarga         ###   ########.fr       */
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

void	coder_think(t_coder *coder)
{
	print_status(coder, "is thinking");
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	coder->last_compile_time = get_time();
	while (1)
	{
		if (coder->comp_count == coder->rules->num_compiles_required)
			break ;
	}
	pthread_mutex_lock(&coder->rules->print_lock);
	printf("Hello from coder %d\n", coder->id);
	pthread_mutex_unlock(&coder->rules->print_lock);
	return (NULL);
}

int	start_sim(t_box *box)
{
	int		i;

	i = 0;
	while (i < box->rules.num_coders)
	{
		pthread_create(&box->threads[i], NULL, coder_routine, &box->coders[i]);
		i++;
	}
	i = 0;
	while (i < box->rules.mum_coders)
	{
		pthread_join(box->threads[i], NULL);
		i++;
	}
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
