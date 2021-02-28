/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_funs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gihwan-kim <kgh06079@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 13:28:33 by gihwan-kim        #+#    #+#             */
/*   Updated: 2021/02/28 12:25:23 by gihwan-kim       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

extern t_option		*g_option;
extern t_philo		*g_philo_array;

void	eat(t_philo *data)
{
	sem_wait(data->sem_my_forks);
	print_message(data, "has taken a fork");
	sem_wait(data->sem_my_forks);
	print_message(data, "has taken a fork");
	if (data->s_last_eat_time == NULL)
		data->s_last_eat_time = (struct timeval *)malloc(sizeof(struct timeval));
	gettimeofday(data->s_last_eat_time, NULL);
	print_message(data, "is eating");
	sem_wait(g_option->sem_state);
	data->eat_count++;
	sem_post(g_option->sem_state);
	usleep(g_option->time_to_eat);
	sem_post(data->sem_my_forks);
	sem_post(data->sem_my_forks);
}

void	*philo_died_check_func(void *_data)
{
	t_philo		*data;

	data = (t_philo*)_data;
	while (1)
	{
		if (data->s_last_eat_time && !check_died(data))
		{
			print_message(data, "died");
			sem_post(g_option->sem_died);
			return (0);
		}
		usleep(100);
	}
	return (0);
}

void	*philo_full_check_func(void *_data)
{
	t_philo		*data;

	data = (t_philo*)_data;
	while (1 && g_option->num_of_philo_must_eat >= 0)
	{
		if (data->eat_count >= g_option->num_of_philo_must_eat)
		{
			sem_wait(g_option->sem_state);
			g_option->is_eat_over = 1;
			sem_post(g_option->sem_state);
			return (0);
		}
	}
	return (0);	
}


/*
** philo_eat_check : died check
** philo_full_check : full check
*/
int		philo_action(t_philo *data)
{
	pthread_t	philo_died_check;
	pthread_t	philo_full_check;

	pthread_create(&philo_died_check, NULL, philo_died_check_func, data);
	pthread_detach(philo_died_check);
	pthread_create(&philo_full_check, NULL, philo_full_check_func, data);
	pthread_detach(philo_full_check);
	while (1 && !g_option->is_eat_over)
	{
		eat(data);
		print_message(data, "is sleeping");
		usleep(g_option->time_to_sleep);
		print_message(data, "is thinking");
	}
	return (g_option->is_died);
}

void	*philo_killer_func(void *_data)
{
	int	i;

	(void)_data;
	sem_wait(g_option->sem_died);
	i = -1;
	while (++i < g_option->num_of_philo)
		kill(g_philo_array[i].processor_idx, SIGINT);
	sem_post(g_option->sem_died);
	return (0);
}

int		start_philosopher()
{
	pthread_t	killer;
	int			i;
	pid_t		ret;

	i = -1;
	ret = 1;
	while (++i < g_option->num_of_philo && ret)
	{
		ret = fork();
		g_philo_array[i].processor_idx = ret;
		if (!ret)
		{
			philo_action(g_philo_array + i);
			exit(0);
		}
		usleep(10);
	}
	pthread_create(&killer, NULL, philo_killer_func, NULL);
	pthread_detach(killer);
	i = -1;
	while (++i < g_option->num_of_philo)
		waitpid(g_philo_array[i].processor_idx, NULL, 0);
	return (1);
}
