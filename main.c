/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dievarga <dievarga@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 10:11:23 by dievarga          #+#    #+#             */
/*   Updated: 2026/07/21 13:46:27 by dievarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	main(int ac, char **av)
{
	t_box	box;

	if (!parse_args(&box, ac, av))
		return (1);
	box.coders = malloc(sizeof(t_coder) * box.rules.num_coders);
	box.dongles = malloc(sizeof(t_dongle) * box.rules.num_coders);
	if (!box.coders || !box.dongles)
	{
		free(box.coders);
		free(box.doongles);
		write(2, "Error: Malloc Failed", 25);
		return (1);
	}
	printf("All Good");
	free(box.coders);
	free(box.dongles;)
	return (0);
}
