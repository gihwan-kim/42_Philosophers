/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gihwan-kim <kgh06079@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 13:14:57 by gihwan-kim        #+#    #+#             */
/*   Updated: 2021/02/28 10:14:30 by gihwan-kim       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

extern t_option		*g_option;

int		ft_isnum(const char *nptr)
{
	while (*nptr)
	{
		if (48 <= *nptr && *nptr <= 57)
			;
		else
			return (0);
		nptr++;
	}
	return (1);
}

int		ft_atoi(const char *nptr)
{
	int minus;
	int num;

	num = 0;
	minus = 1;
	while (*nptr && (*nptr == ' ' || *nptr == '\n' || *nptr == '\t'
				|| *nptr == '\r' || *nptr == '\v' || *nptr == '\f'))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			minus = -minus;
		nptr++;
	}
	while ('0' <= *nptr && *nptr <= '9')
	{
		num = num * 10 + (*nptr - '0');
		nptr++;
	}
	return (minus * num);
}

void	print_message(t_philo *data, const char *str)
{
	struct timeval	s_cur;
	long			cur_time;

	if (data && !g_option->is_died && !g_option->is_eat_over)
	{
		sem_wait(g_option->sem_printf);
		gettimeofday(&s_cur, NULL);
		cur_time = convert_milsec(&s_cur) - convert_milsec(data->s_start_time);
		printf("%ldms %d %s\n", cur_time, data->no, str);
		sem_post(g_option->sem_printf);
	}
	else if (data == NULL)
	{
		sem_wait(g_option->sem_printf);
		printf("%s\n", str);
		sem_post(g_option->sem_printf);
	}
}

int		check_died(t_philo *data)
{
	struct timeval	s_cur;

	gettimeofday(&s_cur, NULL);
	if (convert_milsec(&s_cur) - convert_milsec(data->s_last_eat_time)
		> g_option->time_to_die)
		return (0);
	return (1);
}

long	convert_milsec(struct timeval *time)
{
	return (time->tv_sec * 1000L + time->tv_usec / 1000L);
}
