/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabatist <rabatist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:21:38 by rabatist          #+#    #+#             */
/*   Updated: 2025/02/28 18:25:45 by rabatist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!philo->data->dead)
	{
		printf("[%lld] Philosopher %d thinks\n",
			get_time() - philo->data->start_time, philo->id + 1);
		pthread_mutex_lock(&philo->data->fork[philo->left_fork]);
		printf("[%lld] Philosopher %d took the left fork\n",
			get_time() - philo->data->start_time, philo->id + 1);
		pthread_mutex_lock(&philo->data->fork[philo->right_fork]);
		printf("[%lld] Philosopher %d took the right fork\n",
			get_time() - philo->data->start_time, philo->id + 1);
		pthread_mutex_lock(&philo->data->eating);
		philo->death_timer = get_time();
		philo->count_meal++;
		pthread_mutex_unlock(&philo->data->eating);
		printf("[%lld] Philosopher %d is eating\n",
			get_time() - philo->data->start_time, philo->id + 1);
		good_sleep(philo->data->t_to_eat);
		pthread_mutex_unlock(&philo->data->fork[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->fork[philo->right_fork]);
		printf("[%lld] Philosopher %d sleeps\n",
			get_time() - philo->data->start_time, philo->id + 1);
		good_sleep(philo->data->t_to_sleep);
	}
	return (NULL);
}


int	start_threads(t_data *data)
{
	int			i;
	pthread_t	monitor;

	if (pthread_create(&monitor, NULL, check_death, data))
	{
		printf("Error: impossivle to create monitor thread\n");
		return (0);
	}
	i = -1;
	while (++i < data->n_philo)
	{
		if (pthread_create(&data->philo[i].thread, NULL, philosopher_routine,
				&data->philo[i]))
		{
			printf("Error: impossible to create philo thread %d\n", i);
			return (0);
		}
	}
	i = -1;
	while (++i < data->n_philo)
		pthread_join(data->philo[i].thread, NULL);
	pthread_join(monitor, NULL);
	return (1);
}

void	*check_death(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->n_philo)
		{
			pthread_mutex_lock(&data->eating);
			if (get_time() - data->philo[i].death_timer > data->t_to_die)
			{
				pthread_mutex_unlock(&data->eating);
				printf("[%lld] Philosopher %d died\n",
					get_time() - data->start_time, data->philo[i].id + 1);
				data->dead = 1;
				return (NULL);
			}
			pthread_mutex_unlock(&data->eating);
			i++;
		}
		usleep(500);
	}
	return (NULL);
}
