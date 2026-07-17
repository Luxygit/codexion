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

typedef struct s_coder
{
	int			id;
	int			comp_count;
	t_rules		*rules;
	t_dongle	*l_dongle;
	t_dongle	*r_dongle;
}	t_coder;

typedef struct s_dongle
{
	int		in_use;
	time_t	cooldown;
}	t_dongle;

typedef struct s_box
{
	t_rules		rules;
	t_dongle	*dongles;
	t_coder		*coders;
}	t_box;
