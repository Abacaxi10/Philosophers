/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabatist <rabatist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 17:57:58 by rabatist          #+#    #+#             */
/*   Updated: 2025/02/28 15:28:19 by rabatist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int ac, char **av)
{
	t_data	data;
	int		i;

	if (!check_arg(ac, av, &data))
		return (0);
	if (!init_mutex(&data))
		return (0);
	if (!init_philo(&data))
		return (0);
	if (!start_threads(&data))
		return (0);
	i = -1;
	while (++i < data.n_philo)
		pthread_mutex_destroy(&data.fork[i]);
	pthread_mutex_destroy(&data.message);
	pthread_mutex_destroy(&data.eating);
	free (data.fork);
	free (data.philo);
	return (0);
}
