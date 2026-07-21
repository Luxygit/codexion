/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 test.h												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: dievarga <dievarga@student.42barcelona.co	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/17 22:13:02 by dievarga		   #+#	  #+#			  */
/*	 Updated: 2026/07/17 22:19:29 by dievarga		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <time.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>

typedef struct s_rules
{
	int		num_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		num_compiles_required;
	int		dongle_cooldown;
}	t_rules;

typedef struct s_dongle
{
	int		in_use;
	time_t	cooldown;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	int			comp_count;
	t_rules		*rules;
	t_dongle	*l_dongle;
	t_dongle	*r_dongle;
}	t_coder;

typedef struct s_box
{
	t_rules		rules;
	t_dongle	*dongles;
	t_coder		*coders;
}	t_box;

int			ft_is_num(char *str);
long		ft_atoi_safe(char *str);
int			parse_arguments(t_box *box, int ac, char **av);

#endif
