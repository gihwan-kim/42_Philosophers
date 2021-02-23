/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gihwan-kim <kgh06079@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 09:27:38 by gihwan-kim        #+#    #+#             */
/*   Updated: 2021/02/23 15:35:50 by gihwan-kim       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** left right 순서의 상관관계는 무엇일까
** pthread_mutex_lock(data->fork_left);
** pthread_mutex_lock(data->fork_right);
*/
pthread_mutex_t	*g_m_forks;
t_option		*g_option = NULL;
t_philo			*g_philo_array = NULL;

void	memory_clear()
{
	int	i;

	i = -1;
	while (++i < g_option->num_of_philo)
	{
		free(g_philo_array[i].s_last_eat_time);
		free(g_philo_array[i].s_start_time);
	}
	free(g_philo_array);
	pthread_mutex_destroy(&(g_option->mutex_printf));
	pthread_mutex_destroy(&(g_option->mutex_state));
	free(g_option);
	free(g_m_forks);
}

int		argument_check(char **argv)
{
	int	i;

	i = 0;
	while (argv[++i])
	{
		if (!ft_isnum(argv[i]))
			return (0);
	}
	return (1);
}

int		error_str(char *str)
{
	printf("%s\n", str);
	return (0);
}

int		main(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (error_str("Wrong argument numbber!"));
	if (!argument_check(argv))
		return (error_str("Wrong argument form!"));
	if (!init_option(argc, argv) || !init_mutex())
		return (error_str("Program initialization failed!"));
	if (!(g_philo_array = (t_philo*)malloc(sizeof(t_philo) * g_option->num_of_philo)))
		return (error_str("Program initialization failed!"));
	if (!init_philo_array())
		return (error_str("Program initialization failed!"));;
	create_thread();
	memory_clear();
	return (0);
}
