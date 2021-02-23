/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_funs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gihwan-kim <kgh06079@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 13:28:33 by gihwan-kim        #+#    #+#             */
/*   Updated: 2021/02/23 21:11:19 by gihwan-kim       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

extern pthread_mutex_t	*g_m_forks;
extern t_option		    *g_option;
extern t_philo		    *g_philo_array;

/*
** check num_of_times_philo_must_eat
*/
void	*philo_all_state(void *_data)
{
	int	i;

	(void)_data;
	i = -1;
	while (1 && !g_option->is_died && !g_option->is_eat_over)
	{
		if (g_option->num_of_times_philo_must_eat >= 0 &&
		g_option->eat_num >= g_option->num_of_times_philo_must_eat *
							g_option->num_of_philo)
		{
			pthread_mutex_lock(&g_option->mutex_state);
			g_option->is_eat_over = 1;
			pthread_mutex_unlock(&g_option->mutex_state);
			return (0);
		}
	}
	return (0);
}

void	*philo_state(void *_data)
{
	t_philo		*data;

	data = (t_philo*)_data;
	while (1 && !g_option->is_died && !g_option->is_eat_over)
	{
		if (data->s_last_eat_time && !check_died(data))
		{
			print_message(data, "died");
			pthread_mutex_lock(&g_option->mutex_state);
			g_option->is_died = 1;
			pthread_mutex_unlock(&g_option->mutex_state);
			return (0);
		}
		usleep(100);
	}
	return (0);
}

void	eat(t_philo *data)
{
	pthread_mutex_lock(data->fork_right);
	pthread_mutex_lock(data->fork_left);
	print_message(data, "has taken a fork");
	print_message(data, "has taken a fork");
	if (data->s_last_eat_time == NULL)
		data->s_last_eat_time = (struct timeval *)malloc(sizeof(struct timeval));
	gettimeofday(data->s_last_eat_time, NULL);
	print_message(data, "is eating");
	pthread_mutex_lock(&g_option->mutex_state);
	g_option->eat_num++;
	pthread_mutex_unlock(&g_option->mutex_state);
	usleep(g_option->time_to_eat);
	pthread_mutex_unlock(data->fork_left);
	pthread_mutex_unlock(data->fork_right);
}

void	*philo_action(void *_data)
{
	t_philo		*data;
	pthread_t	philo_eat_check;	

	data = (t_philo*)_data;
	pthread_create(&philo_eat_check, NULL, philo_state, data);
	pthread_detach(philo_eat_check);
	while (1 && !g_option->is_died && !g_option->is_eat_over)
	{
		eat(data);
		print_message(data, "is sleeping");
		usleep(g_option->time_to_sleep);
		print_message(data, "is thinking");
	}
	return (NULL);
}

int		create_thread()
{
	pthread_t	philo_all_check;
	int	i;
	int	ret;

	i = -1;
	ret = 0;
	while (++i < g_option->num_of_philo && !ret)
	{
		ret = pthread_create(&(g_philo_array[i].thread_idx), NULL, philo_action, &g_philo_array[i]);
		usleep(20);
	}
	if (ret)
		return (0);
	ret = pthread_create(&philo_all_check, NULL, philo_all_state, NULL);
	pthread_detach(philo_all_check);
	i = 0;
	while (i < g_option->num_of_philo && !ret)
		ret = pthread_join(g_philo_array[i++].thread_idx, NULL);
	if (ret)
		return (0);
	return (1);
}