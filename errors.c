/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabatist <rabatist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:16:07 by rabatist          #+#    #+#             */
/*   Updated: 2025/03/02 02:02:22 by rabatist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_arg(int ac, char **av, t_data *data)
{
	if (ac != 5 && ac != 6)
	{
		printf("Error : Wrong Number of arguments\n");
		return (0);
	}
	if (!init_arg(ac, av, data))
	{
		printf("Error : Only positiv numbers allowed\n");
		return (0);
	}
	return (1);
}

int	init_arg(int ac, char **av, t_data *data)
{
	int	i;

	i = 1;
	while (ac > 1)
	{
		if (!ft_isdigit(av[i]))
			return (0);
		i++;
		ac--;
	}
	data->dead = 0;
	data->end_routine = 0;
	data->count_meal = 0;
	data->n_philo = ft_atoi(av[1]);
	data->t_to_die = ft_atoi(av[2]);
	data->t_to_eat = ft_atoi(av[3]);
	data->t_to_sleep = ft_atoi(av[4]);
	if (av[5])
		data->n_philo_eat = ft_atoi(av[5]);
	else
		data->n_philo_eat = -1;
	return (1);
}
