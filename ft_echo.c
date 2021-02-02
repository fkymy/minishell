/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 17:25:52 by tayamamo          #+#    #+#             */
/*   Updated: 2021/02/02 18:07:19 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft/libft.h"

int	ft_echo(char *argv[])
{
	int	i;

	if (argv[1] == NULL)
	{
		ft_putstr_fd("\n", 1);
		return (0);
	}
	i = ft_strcmp(argv[1], "-n") ? 1 : 2;
	while (argv[i])
	{
		ft_putstr_fd(argv[i++], 1);
		if (argv[i] != NULL)
			ft_putstr_fd(" ", 1);
	}
	if (ft_strcmp(argv[1], "-n"))
		ft_putstr_fd("\n", 1);
	return (0);
}
