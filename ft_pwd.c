/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 17:26:51 by tayamamo          #+#    #+#             */
/*   Updated: 2021/02/04 12:22:19 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/errno.h>
#include "minishell.h"
#include "libft/libft.h"

char	*get_pwd(void)
{
	if (env_get(g_env, "PWD") == NULL)
		return ("");
	return (env_get(g_env, "PWD")->value);
}

int		ft_pwd(void)
{
	char	*buf;

	if ((buf = getcwd(NULL, 0)) == NULL)
	{
		ft_putstr_fd(get_pwd(), 1);
		ft_putstr_fd("\n", 1);
		return (0);
	}
	ft_putstr_fd(buf, 1);
	ft_putstr_fd("\n", 1);
	free(buf);
	return (0);
}
