/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabatist <rabatist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 20:45:08 by rabatist          #+#    #+#             */
/*   Updated: 2025/03/03 20:47:54 by rabatist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	sleeping(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->message);
	if (!philo->data->dead && !philo->data->end_routine)
		printf("[%lld] Philosopher %d is sleeping\n",
			get_time() - philo->data->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->data->message);
	good_sleep(philo->data->t_to_sleep, philo->data);
}

void	thinking(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->message);
	if (!philo->data->dead && !philo->data->end_routine)
		printf("[%lld] Philosopher %d is thinking\n",
			get_time() - philo->data->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->data->message);
}
