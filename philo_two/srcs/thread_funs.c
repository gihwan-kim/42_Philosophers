/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_funs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gihwan-kim <kgh06079@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 13:28:33 by gihwan-kim        #+#    #+#             */
/*   Updated: 2021/02/28 13:16:33 by gihwan-kim       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

extern t_option		    *g_option;
extern t_philo		    *g_philo_array;

/*
** 바꾼것
** 	'전체 횟수 변수 == 각 철학자별 최소 횟수 * 철학자 수' 를 비교하는 방법대신
** 	각 철학자 별로 횟수를 계산하도록 바꿨다.
** 	전체 수를 비교하다 보니 각 철학자 최소 횟수가 맞지 않는 경우가 생김
*/

/*
** check num_of_times_philo_must_eat
*/

void	*philo_all_state(void *_data)
{
	int	i;
	int	count;

	(void)_data;
	while (1 && g_option->num_of_philo_must_eat >= 0 &&
			!g_option->is_died && !g_option->is_eat_over)
	{
		i = -1;
		count = 0;
		while (++i < g_option->num_of_philo)
		{
			if (g_philo_array[i].eat_count >= g_option->num_of_philo_must_eat)
			{
				count++;
				if (count == g_option->num_of_philo)
				{
					sem_wait(g_option->sem_state);
					g_option->is_eat_over = 1;
					sem_post(g_option->sem_state);
					return (0);
				}
			}
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
			sem_wait(g_option->sem_state);
			g_option->is_died = 1;
			sem_post(g_option->sem_state);
			return (0);
		}
		usleep(100);
	}
	return (0);
}

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
	data->eat_count++;
	usleep(g_option->time_to_eat);
	sem_post(data->sem_my_forks);
	sem_post(data->sem_my_forks);
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
	i = -1;
	while (++i < g_option->num_of_philo)
	{
		pthread_create(&(g_philo_array[i].thread_idx), NULL, philo_action, &g_philo_array[i]);
		usleep(40);
	}
	pthread_create(&philo_all_check, NULL, philo_all_state, NULL);
	pthread_detach(philo_all_check);
	i = 0;
	while (i < g_option->num_of_philo)
		pthread_join(g_philo_array[i++].thread_idx, NULL);
	return (1);
}