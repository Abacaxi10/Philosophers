/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead_or_full.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabatist <rabatist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:00:50 by rabatist          #+#    #+#             */
/*   Updated: 2025/03/11 16:30:03 by rabatist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*check_death_or_full(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (check_any_philosopher_died(data))
			return (NULL);
		if (data->n_philo_eat != -1 && have_all_philo_ate(data))
		{
			all_philo_ate(data);
			return (NULL);
		}
		usleep(50);
	}
	return (NULL);
}

int	check_any_philosopher_died(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
	{
		pthread_mutex_lock(&data->eating);
		if (get_time() - data->philo[i].death_timer > data->t_to_die)
		{
			handle_death(data, i);
			return (1);
		}
		pthread_mutex_unlock(&data->eating);
	}
	return (0);
}

void	handle_death(t_data *data, int philo_id)
{
	pthread_mutex_unlock(&data->eating);
	pthread_mutex_lock(&data->dead_mutex);
	data->dead = 1;
	pthread_mutex_unlock(&data->dead_mutex);
	pthread_mutex_lock(&data->message);
	printf("[%lld] Philosopher %d died\n",
		get_time() - data->start_time, data->philo[philo_id].id + 1);
	pthread_mutex_lock(&data->message);
}

int	have_all_philo_ate(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
	{
		pthread_mutex_lock(&data->eating);
		if (data->philo[i].count_meal < data->n_philo_eat)
		{
			pthread_mutex_unlock(&data->eating);
			return (0);
		}
		pthread_mutex_unlock(&data->eating);
	}
	return (1);
}

void	all_philo_ate(t_data *data)
{
	pthread_mutex_lock(&data->message);
	printf("[%lld] Tous les philosophes ont mangé %d fois\n",
		get_time() - data->start_time + data->t_to_eat, data->n_philo_eat);
	pthread_mutex_unlock(&data->message);
	pthread_mutex_lock(&data->dead_mutex);
	data->end_routine = 1;
	pthread_mutex_unlock(&data->dead_mutex);
	usleep(10000);
}
