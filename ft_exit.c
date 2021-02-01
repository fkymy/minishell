/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 15:31:40 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/01 16:07:27 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int			ft_exit(char *argv[])
{
	int	status;

	status = g_exit_status;
	ft_putstr_fd("exit\n", 2);
	if (ft_strslen(argv) > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (argv[1])
		status = ft_atoi(argv[1]);
	exit(status);
	return (0);
}
