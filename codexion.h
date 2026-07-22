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
# include <pthread.h>
# include <sys/time.h>

typedef struct s_rules
{
	int		num_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		num_compiles_required;
	int		dongle_cooldown;
	int		is_edf;
}	t_rules;

typedef struct s_dongle
{
	int				in_use;
	long long		cooldown;
	pthread_mutex_t	lock;
	int				waiting_queue[2];
	int				queue_size;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	int				comp_count;
	time_t			last_compile_time;
	t_rules			*rules;
	struct s_box	*box;
	t_dongle		*l_dongle;
	t_dongle		*r_dongle;
}	t_coder;

typedef struct s_box
{
	t_rules			rules;
	t_dongle		*dongles;
	t_coder			*coders;
	int				sim_stopped;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	stop_lock;
	pthread_t		*threads;
}	t_box;

int			ft_is_num(char *str);
long long	ft_atoi_safe(char *str);
int			parse_args(t_box *box, int ac, char **av);
int			init_data(t_box *box);
int			start_sim(t_box *box);
long long	get_time(void);
void		ft_usleep(long long milliseconds, tbox *box);
int			check_sim_status(tbox *box);
int			is_dongle_cooling(t_dongle *dongle);
int			try_get_dongle(t_dongle *dongle);
int			start_sim(t_box *box);
void		enqueue_coder(t_dongle *dongle, int coder_id, t_box *box);
void		dequeue_coder(t_dongle *dongle);
void		coder_compile(t_coder *coder);
void		coder_debug(t_coder *coder);
void		coder_refactor(t_coder *coder);
int			take_both_dongles(t_coder *coder);

#endif
