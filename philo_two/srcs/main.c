/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gihwan-kim <kgh06079@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 09:27:38 by gihwan-kim        #+#    #+#             */
/*   Updated: 2021/02/28 13:15:39 by gihwan-kim       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

t_option		*g_option = NULL;
t_philo			*g_philo_array = NULL;

void	memory_clear()
{
	int	i;

	i = -1;
	while (++i < g_option->num_of_philo)
	{
		if (g_philo_array[i].s_last_eat_time)
			free(g_philo_array[i].s_last_eat_time);
		if (g_philo_array[i].s_start_time)
			free(g_philo_array[i].s_start_time);
	}
	sem_unlink("sem_forks");
	sem_unlink("sem_printf");
	sem_unlink("sem_state");
	free(g_philo_array);
	free(g_option);
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

int		error_str(char *str, int type)
{
	int	i;

	i = -1;
	printf("%s\n", str);
	if (!type)
		return (0);
	else if (type == 2)
	{
		while (++i < g_option->num_of_philo)
		{
			if (g_philo_array[i].s_start_time)
				free(g_philo_array[i].s_start_time);
			if (g_philo_array[i].s_start_time)
				free(g_philo_array[i].s_start_time);
		}
		free(g_philo_array);
	}
	free(g_option);
	sem_unlink("sem_forks");
	sem_unlink("sem_printf");
	sem_unlink("sem_state");
	return (0);
}

int		main(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (error_str("Wrong argument numbber!", 0));
	if (!argument_check(argv))
		return (error_str("Wrong argument form!", 0));
	if (ft_atoi(argv[1]) <= 1)
		return (error_str("Wrong argument form!", 0));
	if (argc == 6 && !ft_atoi(argv[5]))
		return (error_str("Wrong argument form!", 0));
	if (!init_option(argc, argv) || !init_sema())
		return (error_str("Program initialization !", 0));
	if (!(g_philo_array = (t_philo*)malloc(sizeof(t_philo) * g_option->num_of_philo)))
		return (error_str("Program  failed!", 1));
	if (!init_philo_array())
		return (error_str("Program initialization failed!", 2));
	create_thread();
	memory_clear();
	return (0);
}
