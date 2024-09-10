/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_mutex_thread.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 22:17:54 by miguandr          #+#    #+#             */
/*   Updated: 2024/09/10 13:48:31 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	mutex_error(int status, t_mtx_type type)
{
	if (status == EINVAL && (type == LOCK || type == UNLOCK || type == DESTROY))
		return (ft_error2(1));
	else if (status == EINVAL && type == INIT)
		return (ft_error2(2));
	else if (status == EDEADLK)
		return (ft_error2(3));
	else if (status == EPERM)
		return (ft_error2(4));
	else if (status == ENOMEM)
		return (ft_error2(5));
	else if (status == EBUSY)
		return (ft_error2(6));
	else
		return (0);
}

int	mutex_functions(t_mtx *mutex, t_mtx_type type)
{
	int	status;

	if (type == LOCK)
		status = pthread_mutex_lock(mutex);
	else if (type == UNLOCK)
		status = pthread_mutex_unlock(mutex);
	else if (type == INIT)
		status = pthread_mutex_init(mutex, NULL);
	else if (type == DESTROY)
		status = pthread_mutex_destroy(mutex);
	else
		return (ft_error2(0));
	return (mutex_error(status, type));
}

int	thread_error(int status, t_mtx_type type)
{
	if (status == EAGAIN)
		return (ft_error2(8));
	else if (status == EPERM)
		return (ft_error2(9));
	if (status == EINVAL && type == CREATE)
		return (ft_error2(10));
	else if (status == EINVAL && (type == INIT || type == DETACH))
		return (ft_error2(11));
	else if (status == ESRCH)
		return (ft_error2(12));
	else if (status == EDEADLK)
		return (ft_error2(13));
	else
		return (0);
}

int	thread_funtions(pthread_t *thread, void *(*foo)(void *),
	void *data, t_mtx_type type)
{
	if (type == CREATE)
		return (thread_error(pthread_create(thread, NULL, foo, data), type));
	if (type == JOIN)
		return (thread_error(pthread_join(*thread, NULL), type));
	else if (type == DETACH)
		return (thread_error(pthread_detach(*thread), type));
	else
		return (ft_error2(7));
}
