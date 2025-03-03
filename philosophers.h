/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabatist <rabatist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:21:37 by rabatist          #+#    #+#             */
/*   Updated: 2025/03/03 20:48:33 by rabatist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				count_meal;
	long long		death_timer;
	struct s_data	*data;
	pthread_t		thread;
}					t_philo;

typedef struct s_data
{
	int					n_philo;
	int					t_to_die;
	int					t_to_eat;
	int					t_to_sleep;
	int					n_philo_eat;
	int					dead;
	int					end_routine;
	int					count_meal;
	pthread_mutex_t		*fork;
	pthread_mutex_t		message;
	pthread_mutex_t		eating;
	pthread_mutex_t		dead_mutex;
	t_philo				*philo;
	long long			start_time;
}						t_data;

//utils.c
int			ft_atoi(char *str);
int			ft_isdigit(char *str);
long long	get_time(void);
void		good_sleep(long long mili, t_data *data);

// init.c
void		free_mutex(int i, t_data *data);
int			init_philo(t_data *data);
int			init_mutex(t_data *data);
int			malloc_struct(t_data *data);

//errors.c
int			check_arg(int ac, char **av, t_data *data);
int			init_arg(int ac, char **av, t_data *data);

//start_threads.c
void		*philosopher_routine(void *arg);
int			start_threads(t_data *data);

//death_or _full.c
void		*check_death_or_full(void *arg);
int			check_any_philosopher_died(t_data *data);
void		handle_death(t_data *data, int philo_id);
int			have_all_philo_ate(t_data *data);
void		all_philo_ate(t_data *data);

//routine.c
void		*handle_single_philo(t_philo *philo);
int			check_if_dead_or_full(t_philo *philo);
int			take_right_fork(t_philo *philo);
int			take_left_fork(t_philo *philo);
void		eating(t_philo *philo);

//routine2.c
void		sleeping(t_philo *philo);
void		thinking(t_philo *philo);

#endif