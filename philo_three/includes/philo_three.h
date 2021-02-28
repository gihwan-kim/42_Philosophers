/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gihwan-kim <kgh06079@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 09:28:37 by gihwan-kim        #+#    #+#             */
/*   Updated: 2021/02/28 11:06:36 by gihwan-kim       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TWO_H
#define PHILO_TWO_H

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <semaphore.h>
#include <sys/types.h>
#include <signal.h>

/*
** time_to_* : milliseconds
** is_died
** 	1 : yes
**	0 : no
** eat over
**	1 : all philosophers eat at least 'num_of_times_philo_must_eat'
**	0 : no, default
*/
typedef struct		s_option
{
	int				num_of_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				num_of_philo_must_eat;
	int				is_died;
	int				is_eat_over;
	int				eat_num;
	sem_t			*sem_forks;
	sem_t			*sem_printf;
	sem_t			*sem_died;
	sem_t			*sem_state;
}					t_option;

/*
** status
** 	0 : not yet eat
** 	1 : eat number > 0
*/
typedef struct		s_philo
{
	int				no;
	int				eat_count;
	pid_t			processor_idx;
	sem_t			*sem_my_forks;
	struct timeval	*s_start_time;
	struct timeval	*s_last_eat_time;
}					t_philo;

/*
** utils.c
*/
int					ft_atoi(const char *nptr);
void				print_message(t_philo *data, const char *str);
long				convert_milsec(struct timeval *time);
int					check_died(t_philo *data);
int					ft_isnum(const char *nptr);

/*
** init.c
*/
int					init_option(int argc, char **argv);
int					init_sema();
int					init_philo_array();

/*
** thread_funcs.c
*/
void				*philo_all_state(void *_data);
void				*philo_state(void *_data);
int					philo_action(t_philo *_data);
int					start_philosopher();
#endif