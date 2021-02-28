/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gihwan-kim <kgh06079@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 13:23:23 by gihwan-kim        #+#    #+#             */
/*   Updated: 2021/02/28 12:57:12 by gihwan-kim       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

extern t_option		    *g_option;
extern t_philo		    *g_philo_array;

int		init_option(int argc, char **argv)
{
	if (!(g_option = (t_option*)malloc(sizeof(t_option))))
		return (0);
	g_option->num_of_philo = ft_atoi(argv[1]);
	g_option->time_to_die = ft_atoi(argv[2]);
	g_option->time_to_eat = ft_atoi(argv[3]) * 1000;
	g_option->time_to_sleep = ft_atoi(argv[4]) * 1000;
	g_option->is_died = 0;
	g_option->is_eat_over = 0;
	g_option->eat_num = 0;
	if (argc == 6)
		g_option->num_of_philo_must_eat = ft_atoi(argv[5]);
	else
		g_option->num_of_philo_must_eat = -1;
	g_option->sem_forks = NULL;
	g_option->sem_printf = NULL;
	g_option->sem_state = NULL;
	return (1);
}

int		init_sema_clear(int type)
{
	free(g_option);
	if (type == 0)
		return (0);
	else if (type == 1)
		sem_unlink("/sem_forks");
	else if (type == 2)
	{
		sem_unlink("/sem_forks");
		sem_unlink("/sem_printf");
	}
	return (0);
}

int		init_sema()
{
	sem_unlink("/sem_forks");
	sem_unlink("/sem_printf");
	sem_unlink("/sem_state");
	g_option->sem_forks = sem_open("/sem_forks", O_CREAT, 
								S_IWUSR | S_IWGRP | S_IWOTH,
								g_option->num_of_philo);
	if (g_option->sem_forks < 0)
		return (init_sema_clear(0));
	g_option->sem_printf = sem_open("/sem_printf", O_CREAT, 
								S_IWUSR | S_IWGRP | S_IWOTH, 1);
	if (g_option->sem_printf < 0)
		return (init_sema_clear(1));
	g_option->sem_state = sem_open("/sem_state", O_CREAT, 
								S_IWUSR | S_IWGRP | S_IWOTH, 1);
	if (g_option->sem_state < 0)
		return (init_sema_clear(2));
	return (1);
}

int		init_philo_array()
{
	int	ret;
	int	i;

	i = -1;
	ret = 0;
	while (++i < g_option->num_of_philo && !ret)
	{
		g_philo_array[i].eat_count = 0;
		g_philo_array[i].no = i + 1;
		g_philo_array[i].sem_my_forks = g_option->sem_forks;
		g_philo_array[i].s_start_time = (struct timeval *)malloc(sizeof(struct timeval));
		if (!g_philo_array[i].s_start_time)
			return (0);
		g_philo_array[i].s_last_eat_time = NULL;
		ret = gettimeofday(g_philo_array[i].s_start_time, NULL);
	}
	if (ret)
		return (0);
	return (1);
}