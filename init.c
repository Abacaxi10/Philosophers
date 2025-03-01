/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabatist <rabatist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:14:05 by rabatist          #+#    #+#             */
/*   Updated: 2025/03/01 18:21:14 by rabatist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_mutex(int i, t_data *data)
{
	while (i >= 0)
	{
		pthread_mutex_destroy(&(data->fork[i]));
		i--;
	}
}

int	init_philo(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
	{
		data->philo[i].id = i;
		data->philo[i].left_fork = i;
		if (i == data->n_philo - 1)
			data->philo[i].right_fork = 0;
		else
			data->philo[i].right_fork = (i + 1);
		data->philo[i].death_timer = 0;
		data->philo[i].count_meal = 0;
		data->philo[i].data = data;
	}
	i = -1;
	data->start_time = get_time();
	while (++i < data->n_philo)
		data->philo[i].death_timer = data->start_time;
	return (1);
}

int	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	if (!malloc_struct(data))
		return (0);
	while (i < data->n_philo)
	{
		if (pthread_mutex_init(&(data->fork[i]), NULL))
		{
			free_mutex(i - 1, data);
			return (0);
		}
		i++;
	}
	if (pthread_mutex_init(&(data->message), NULL)
		|| pthread_mutex_init(&(data->eating), NULL)
		|| pthread_mutex_init(&(data->dead_mutex), NULL))
		return (0);
	return (1);
}

int	malloc_struct(t_data *data)
{
	data->fork = malloc(sizeof(pthread_mutex_t) * data->n_philo);
	if (!data->fork)
	{
		printf("Error: malloc failed for forks\n");
		return (0);
	}
	data->philo = malloc(sizeof(t_philo) * data->n_philo);
	if (!data->philo)
	{
		printf("Error: malloc failed for philo");
		free (data->fork);
		return (0);
	}
	return (1);
}
