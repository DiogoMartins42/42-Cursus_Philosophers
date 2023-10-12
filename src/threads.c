/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmanuel- <dmanuel-@student.42porto.pt>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 11:11:20 by dmanuel-          #+#    #+#             */
/*   Updated: 2023/09/19 12:36:06 by dmanuel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*monitor(void *data)
{
	t_philo	*philo;

	philo = (t_philo *) data;
	pthread_mutex_lock(&philo->data->write);
	printf("data val: %d\n", philo->data->dead);
	pthread_mutex_unlock(&philo->data->write);
	pthread_mutex_lock(&philo->data->lock);
	while (philo->data->dead == 0)
	{
		pthread_mutex_unlock(&philo->data->lock);
		pthread_mutex_lock(&philo->data->lock);
		if (philo->data->finished >= philo->data->philos_num)
		{
			philo->data->dead = 1;
		}
		pthread_mutex_unlock(&philo->data->lock);
		pthread_mutex_lock(&philo->data->lock);
	}
	pthread_mutex_unlock(&philo->data->lock);
	return ((void *)0);
}

void	*watcher(void *philo_p)
{
	t_philo	*philo;

	philo = (t_philo *) philo_p;
	pthread_mutex_lock(&philo->data->lock);
	while (philo->data->dead == 0)
	{
		pthread_mutex_unlock(&philo->data->lock);
		pthread_mutex_lock(&philo->lock);
		if (get_time() >= philo->time_to_die && philo->eating == 0)
			typing(DIED, philo);
		if (philo->eat == philo->data->eaten)
		{
			pthread_mutex_lock(&philo->data->lock);
			philo->data->finished++;
			philo->eat++;
			pthread_mutex_unlock(&philo->data->lock);
		}
		pthread_mutex_unlock(&philo->lock);
		pthread_mutex_lock(&philo->data->lock);
	}
	pthread_mutex_unlock(&philo->data->lock);
	return ((void *)0);
}

void	*routine(void *philo_p)
{
	t_philo	*philo;

	philo = (t_philo *) philo_p;
	pthread_mutex_lock(&philo->data->lock);
	philo->time_to_die = philo->data->death_time + philo->data->start_time;
	pthread_mutex_unlock(&philo->data->lock);
	if (pthread_create(&philo->thread1, NULL, &watcher, \
	(void *)philo))
		return ((void *)1);
	if (pthread_detach(philo->thread1))
		return ((void *)1);
	pthread_mutex_lock(&philo->data->lock);
	while (philo->data->dead == 0 && philo->data->solo == 0)
	{
		pthread_mutex_unlock(&philo->data->lock);
		eat(philo);
		typing(THINKING, philo);
		pthread_mutex_lock(&philo->data->lock);
	}
	pthread_mutex_unlock(&philo->data->lock);
	return ((void *)0);
}

int	thread_init(t_data *data)
{
	int			i;
	pthread_t	t0;

	i = -1;
	data->start_time = get_time();
	if (data->eaten > 0)
	{
		if (pthread_create(&t0, NULL, &monitor, &data->philos[0]))
			return (error(TH_ERR, data));
		pthread_detach(t0);
	}
	while (++i < data->philos_num)
	{
		if (pthread_create(&data->tid[i], NULL, &routine, &data->philos[i]))
			return (error(TH_ERR, data));
		usleep(1000);
	}
	i = -1;
	while (++i < data->philos_num)
	{
		if (pthread_join(data->tid[i], NULL))
			return (error(JOIN_ERR, data));
	}
	return (0);
}
