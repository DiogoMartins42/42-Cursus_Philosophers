/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmanuel- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 14:28:18 by dmanuel-          #+#    #+#             */
/*   Updated: 2023/08/31 12:26:35 by dmanuel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>

struct	s_data;

typedef struct s_philo
{
	struct s_data		*data;
	int					id;
	int					eat;
	int					status;
	int					eating;
	u_int64_t			time_to_die;
	pthread_t 			thread1;
	pthread_mutex_t		lock;
	pthread_mutex_t		*rfork;
	pthread_mutex_t		*lfork;
}	t_philo;

typedef struct s_data
{
	pthread_mutex_t		*forks;
	pthread_mutex_t		lock;
	pthread_mutex_t		write;
	pthread_t			*tid;
	int					philos_num;
	int					eaten;
	int					dead;
	int					finished;
	u_int64_t			start_time;
	u_int64_t			eat_time;
	u_int64_t			sleep_time;
	u_int64_t			death_time;
	t_philo				*philos;
}	t_data;

int		error(char *str, t_data *data);
void	ft_exit(t_data *data);
int		init(t_data *data, char **argv, int argc);
int		thread_init(t_data *data);
#endif
