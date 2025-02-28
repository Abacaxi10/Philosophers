/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_do.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabatist <rabatist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:05:23 by rabatist          #+#    #+#             */
/*   Updated: 2025/02/28 16:11:04 by rabatist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	philo_do_even(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->fork[philo->right_fork]);
	printf("[%lld] Philosopher %d took the right fork\n",
		get_time() - philo->data->start_time, philo->id + 1);
	pthread_mutex_lock(&philo->data->fork[philo->left_fork]);
	printf("[%lld] Philosopher %d took the left fork\n",
		get_time() - philo->data->start_time, philo->id + 1);
}

void	philo_do_odd(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->fork[philo->left_fork]);
	printf("[%lld] Philosopher %d took the left fork\n",
		get_time() - philo->data->start_time, philo->id + 1);
	pthread_mutex_lock(&philo->data->fork[philo->right_fork]);
	printf("[%lld] Philosopher %d took the right fork\n",
		get_time() - philo->data->start_time, philo->id + 1);
}
