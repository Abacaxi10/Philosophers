/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabatist <rabatist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:21:38 by rabatist          #+#    #+#             */
/*   Updated: 2025/03/02 02:48:05 by rabatist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);

	while (1)
	{
		pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->dead || philo->data->end_routine)
		{
			pthread_mutex_unlock(&philo->data->dead_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->dead_mutex);
		pthread_mutex_lock(&philo->data->fork[philo->right_fork]);
		pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->dead || philo->data->end_routine)
		{
			pthread_mutex_unlock(&philo->data->dead_mutex);
			pthread_mutex_unlock(&philo->data->fork[philo->right_fork]);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->dead_mutex);
		pthread_mutex_lock(&philo->data->message);
		if (!philo->data->dead && !philo->data->end_routine)
			printf("[%lld] Philosopher %d has taken the right fork\n",
				get_time() - philo->data->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->data->message);
		if (philo->data->n_philo == 1)
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
		}
		pthread_mutex_lock(&philo->data->fork[philo->left_fork]);
		pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->dead || philo->data->end_routine)
		{
			pthread_mutex_unlock(&philo->data->dead_mutex);
			pthread_mutex_unlock(&philo->data->fork[philo->left_fork]);
			pthread_mutex_unlock(&philo->data->fork[philo->right_fork]);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->dead_mutex);
		pthread_mutex_lock(&philo->data->message);
		if (!philo->data->dead && !philo->data->end_routine)
			printf("[%lld] Philosopher %d has taken the left fork\n",
				get_time() - philo->data->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->data->message);
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
		pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->dead || philo->data->end_routine)
		{
			pthread_mutex_unlock(&philo->data->dead_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->dead_mutex);
		pthread_mutex_lock(&philo->data->message);
		if (!philo->data->dead && !philo->data->end_routine)
			printf("[%lld] Philosopher %d is sleeping\n",
				get_time() - philo->data->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->data->message);
		good_sleep(philo->data->t_to_sleep, philo->data);
		pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->dead || philo->data->end_routine)
		{
			pthread_mutex_unlock(&philo->data->dead_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->dead_mutex);
		pthread_mutex_lock(&philo->data->message);
		if (!philo->data->dead && !philo->data->end_routine)
			printf("[%lld] Philosopher %d is thinking\n",
				get_time() - philo->data->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->data->message);
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
	int		all_ate;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < data->n_philo)
		{
			pthread_mutex_lock(&data->eating);
			if (get_time() - data->philo[i].death_timer > data->t_to_die)
			{
				pthread_mutex_unlock(&data->eating);
				pthread_mutex_lock(&data->dead_mutex);
				data->dead = 1;
				pthread_mutex_unlock(&data->dead_mutex);
				printf("[%lld] Philosopher %d died\n",
					get_time() - data->start_time, data->philo[i].id + 1);
				return (NULL);
			}
			pthread_mutex_unlock(&data->eating);
		}
		if (data->n_philo_eat != -1)
		{
			all_ate = 1;
			i = -1;
			while (++i < data->n_philo)
			{
				pthread_mutex_lock(&data->eating);
				if (data->philo[i].count_meal < data->n_philo_eat)
				{
					all_ate = 0;
					pthread_mutex_unlock(&data->eating);
					break ;
				}
				pthread_mutex_unlock(&data->eating);
			}
			if (all_ate)
			{
				pthread_mutex_lock(&data->message);
				printf("[%lld] Tous les philosophes ont mangé %d fois\n", 
					get_time() - data->start_time + data->t_to_eat, data->n_philo_eat);
				pthread_mutex_unlock(&data->message);
				pthread_mutex_lock(&data->dead_mutex);
				data->end_routine = 1;
				pthread_mutex_unlock(&data->dead_mutex);
				usleep(10000);
				return (NULL);
			}
		}
		usleep(50);
	}
	return (NULL);
}
