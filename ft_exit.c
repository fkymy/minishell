/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 15:31:40 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/09 19:35:29 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int			ft_isdigits(char *s)
{
	while (ft_isspace(*s))
		s++;
	if (*s == '-' || *s == '+')
		s++;
	while (ft_isspace(*s))
		s++;
	if (*s == '\0')
		return (0);
	while (*s)
	{
		if (!ft_isdigit(*s) && !ft_isspace(*s))
			return (0);
		s++;
	}
	return (1);
}

int			get_status(char *str)
{
	long	status;

	status = ft_atol(str);
	if (status == -1 && ft_strcmp(str, "-1") != 0)
		status = 255;
	return ((int)status);
}

int			ft_exit(char *argv[])
{
	ft_putstr_fd("exit\n", 2);
	if (ft_strslen(argv) > 2 && ft_isdigits(argv[1]))
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (argv[1])
	{
		if (ft_isdigits(argv[1]))
			g_exit_status = get_status(argv[1]);
		else
			g_exit_status = 255;
	}
	if (g_exit_status == 255)
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
	exit(g_exit_status);
	return (0);
}
