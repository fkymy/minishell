/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 13:56:34 by tayamamo          #+#    #+#             */
/*   Updated: 2021/02/09 19:37:40 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

void	handler(int signum)
{
	int	save_errno;

	save_errno = errno;
	ft_putstr_fd("\033[2D\033[0K", 2);
	if (signum == SIGINT)
	{
		g_exit_status = 1;
		g_interrupt = signum;
		ft_putstr_fd("\nminishell>", 2);
	}
	errno = save_errno;
}

void	set_signal_handler(void (*func)(int))
{
	if (signal(SIGINT, func) == SIG_ERR)
		die(strerror(errno));
	if (signal(SIGQUIT, func) == SIG_ERR)
		die(strerror(errno));
}
