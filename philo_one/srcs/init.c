/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gihwan-kim <kgh06079@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 13:23:23 by gihwan-kim        #+#    #+#             */
/*   Updated: 2021/02/23 15:35:53 by gihwan-kim       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

extern pthread_mutex_t	*g_m_forks;
extern t_option		    *g_option;
extern t_philo		    *g_philo_array;

int		init_option(int argc, char **argv)
{
	g_option = (t_option*)malloc(sizeof(t_option));
	g_option->num_of_philo = ft_atoi(argv[1]);
	g_option->time_to_die = ft_atoi(argv[2]);
	g_option->time_to_eat = ft_atoi(argv[3]) * 1000;
	g_option->time_to_sleep = ft_atoi(argv[4]) * 1000;
	g_option->is_died = 0;
	g_option->is_eat_over = 0;
	g_option->eat_num = 0;
	if (argc == 6)
		g_option->num_of_times_philo_must_eat = ft_atoi(argv[5]);
	else
		g_option->num_of_times_philo_must_eat = -1;
	g_m_forks = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * g_option->num_of_philo);
	if (!g_m_forks)
		return (0);
	return (1);
}

int		init_mutex()
{
	int	i;
	int	ret;

	i = -1;
	ret = 0;
	while (++i < g_option->num_of_philo && !ret)
		ret = pthread_mutex_init(&g_m_forks[i], NULL);
	if (ret)
		return (0);
	if (pthread_mutex_init(&g_option->mutex_printf, NULL))
		return (0);
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
		g_philo_array[i].no = i + 1;
		g_philo_array[i].fork_left = &g_m_forks[i];
		if (i - 1 < 0)
			g_philo_array[i].fork_right = &g_m_forks[g_option->num_of_philo - 1];
		else
			g_philo_array[i].fork_right = &g_m_forks[i - 1];
		g_philo_array[i].s_start_time = (struct timeval *)malloc(sizeof(struct timeval));
		g_philo_array[i].s_last_eat_time = NULL;
		ret = gettimeofday(g_philo_array[i].s_start_time, NULL);
	}
	if (ret)
		return (0);
	return (1);
}
