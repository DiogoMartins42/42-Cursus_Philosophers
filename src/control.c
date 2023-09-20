/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmanuel- <dmanuel-@student.42porto.pt>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 12:36:48 by dmanuel-          #+#    #+#             */
/*   Updated: 2023/09/19 14:07:14 by dmanuel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

u_int64_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (error("gettimeofday() FAILURE\n", NULL));
	return ((tv.tv_sec * (u_int64_t)1000) + (tv.tv_usec / 1000));
}

void	typing(char *str, t_philo *philo)
{
	u_int64_t	time;

	pthread_mutex_lock(&philo->data->write);
	time = get_time() - philo->data->start_time;
	if (ft_strcmp(DIED, str) == 0 && philo->data->dead == 0)
	{
		printf("%lu %d %s\n", time, philo->id, str);
		philo->data->dead = 1;
	}
	if (!philo->data->dead)
		printf("%lu %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->data->write);
}

void	pick_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->rfork);
	typing(PICKS_FORKS, philo);
	pthread_mutex_lock(philo->lfork);
	typing(PICKS_FORKS, philo);
}

void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->rfork);
	pthread_mutex_unlock(philo->lfork);
	typing(SLEEP, philo);
	ft_usleep(philo, philo->data->sleep_time);
}

void	eat(t_philo *philo)
{
	pick_forks(philo);
	pthread_mutex_lock(&philo->lock);
	philo->eating = 1;
	philo->time_to_die = get_time() + philo->data->death_time;
	typing(EATING, philo);
	philo->eat++;
	ft_usleep(philo, philo->data->eat_time);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->lock);
	drop_forks(philo);
}
