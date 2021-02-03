/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 15:31:40 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/03 14:13:59 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int			ft_isdigits(char *s)
{
	if (*s == '-')
		s++;
	if (*s == '\0')
		return (0);
	while (*s)
		if (!ft_isdigit(*s++))
			return (0);
	return (1);
}

int			ft_exit(char *argv[])
{
	ft_putstr_fd("exit\n", 2);
	if (ft_strslen(argv) > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (argv[1])
	{
		if (ft_isdigits(argv[1]))
			g_exit_status = ft_atoi(argv[1]);
		else
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
			g_exit_status = 255;
		}
	}
	exit(g_exit_status);
	return (0);
}
