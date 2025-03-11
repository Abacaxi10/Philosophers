/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabatist <rabatist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:21:38 by rabatist          #+#    #+#             */
/*   Updated: 2025/03/11 16:35:55 by rabatist         ###   ########.fr       */
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
		if (check_if_dead_or_full(philo))
			return (NULL);
		if (take_right_fork(philo))
			return (NULL);
		if (philo->data->n_philo == 1)
			return (handle_single_philo(philo));
		if (take_left_fork(philo))
			return (NULL);
		eating(philo);
		if (check_if_dead_or_full(philo))
			return (NULL);
		sleeping(philo);
		if (check_if_dead_or_full(philo))
			return (NULL);
		thinking(philo);
	}
	return (NULL);
}

int	start_threads(t_data *data)
{
	int			i;
	pthread_t	monitor;

	if (pthread_create(&monitor, NULL, check_death_or_full, data))
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
