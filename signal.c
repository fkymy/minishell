/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 13:56:34 by tayamamo          #+#    #+#             */
/*   Updated: 2021/01/22 14:00:47 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/errno.h>
#include "libft/libft.h"
#include "minishell.h"

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		if (g_pid == 0)
		{
			ft_putstr_fd("\033[2D\033[0K", 2);
			ft_putstr_fd("\nminishell>", 2);
		}
		else
			ft_putstr_fd("\n", 2);
	}
	if (signum == SIGQUIT)
	{
		if (g_pid == 0)
			ft_putstr_fd("\033[2D\033[0K", 2);
		else
		{
			ft_putstr_fd("Quit: ", 2);
			ft_putnbr_fd(SIGQUIT, 2);
			ft_putstr_fd("\n", 2);
		}
	}
}

void	handle_signals(void)
{
	if (signal(SIGINT, signal_handler) == SIG_ERR)
		die(strerror(errno));
	if (signal(SIGQUIT, signal_handler) == SIG_ERR)
		die(strerror(errno));
}
