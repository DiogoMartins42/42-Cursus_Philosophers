/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initial.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmanuel- <dmanuel-@student.42porto.pt>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 11:47:30 by dmanuel-          #+#    #+#             */
/*   Updated: 2023/09/19 11:09:49 by dmanuel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	alloc(t_data *data)
{
	data->tid = malloc(sizeof(pthread_t) * data->philos_num);
	if (!data->tid)
		return (error(ALLOC_ERR_1, data));
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philos_num);
	if (!data->forks)
		return (error(ALLOC_ERR_2, data));
	data->philos = malloc(sizeof(t_philo) * data->philos_num);
	if (!data->philos)
		return (error(ALLOC_ERR_3, data));
	return (0);
}

int	init_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philos_num)
		pthread_mutex_init(&data->forks[i], NULL);
	i = 0;
	data->philos[0].lfork = &data->forks[0];
	data->philos[0].rfork = &data->forks[data->philos_num - 1];
	i = 1;
	while (i < data->philos_num)
	{
		data->philos[i].rfork = &data->forks[i];
		data->philos[i].lfork = &data->forks[i - 1];
		i++;
	}
	return (0);
}

void	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos_num)
	{
		data->philos[i].data = data;
		data->philos[i].id = i + 1;
		data->philos[i].time_to_die = data->death_time;
		data->philos[i].eat = 0;
		data->philos[i].eating = 0;
		data->philos[i].status = 0;
		pthread_mutex_init(&data->philos[i].lock, NULL);
		i++;
	}
}

int	init_data(t_data *data, char **argv, int argc)
{
	data->philos_num = (int) ft_atoi(argv[1]);
	data->death_time = (u_int64_t) ft_atoi(argv[2]);
	data->eat_time = (u_int64_t) ft_atoi(argv[3]);
	data->sleep_time = (u_int64_t) ft_atoi(argv[4]);
	if (argc == 6)
		data->eaten = (int) ft_atoi(argv[5]);
	else
		data->eaten = -1;
	if (data->philos_num <= 0 || data->philos_num > 200 || \
	data->death_time <= 0 || data->eat_time <= 0 || data->sleep_time <= 0)
		return (error(ERR_2, NULL));
	data->dead = 0;
	data->finished = 0;
	pthread_mutex_init(&data->write, NULL);
	pthread_mutex_init(&data->lock, NULL);
	return (0);
}

int	init(t_data *data, char **argv, int argc)
{
	if (init_data(data, argv, argc))
		return (1);
	if (alloc(data))
		return (1);
	if (init_forks(data))
		return (1);
	init_philos(data);
	return (0);
}
