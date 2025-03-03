/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabatist <rabatist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 19:32:08 by rabatist          #+#    #+#             */
/*   Updated: 2025/03/03 20:44:11 by rabatist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*handle_single_philo(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->dead)
		{
			pthread_mutex_unlock(&philo->data->dead_mutex);
			pthread_mutex_unlock(&philo->data->fork[philo->right_fork]);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->dead_mutex);
		usleep(1000);
	}
	return (NULL);
}

int	check_if_dead_or_full(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead || philo->data->end_routine)
	{
		pthread_mutex_unlock(&philo->data->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->dead_mutex);
	return (0);
}

int	take_right_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->fork[philo->right_fork]);
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead || philo->data->end_routine)
	{
		pthread_mutex_unlock(&philo->data->dead_mutex);
		pthread_mutex_unlock(&philo->data->fork[philo->right_fork]);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->dead_mutex);
	pthread_mutex_lock(&philo->data->message);
	if (!philo->data->dead && !philo->data->end_routine)
		printf("[%lld] Philosopher %d has taken the right fork\n",
			get_time() - philo->data->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->data->message);
	return (0);
}

int	take_left_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->fork[philo->left_fork]);
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead || philo->data->end_routine)
	{
		pthread_mutex_unlock(&philo->data->dead_mutex);
		pthread_mutex_unlock(&philo->data->fork[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->fork[philo->right_fork]);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->dead_mutex);
	pthread_mutex_lock(&philo->data->message);
	if (!philo->data->dead && !philo->data->end_routine)
		printf("[%lld] Philosopher %d has taken the left fork\n",
			get_time() - philo->data->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->data->message);
	return (0);
}

void	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->eating);
	philo->death_timer = get_time();
	philo->count_meal++;
	pthread_mutex_unlock(&philo->data->eating);
	pthread_mutex_lock(&philo->data->message);
	if (!philo->data->dead)
		printf("[%lld] Philosopher %d is eating\n",
			get_time() - philo->data->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->data->message);
	good_sleep(philo->data->t_to_eat, philo->data);
	pthread_mutex_unlock(&philo->data->fork[philo->left_fork]);
	pthread_mutex_unlock(&philo->data->fork[philo->right_fork]);
}
