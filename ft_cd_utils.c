/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 20:46:09 by tayamamo          #+#    #+#             */
/*   Updated: 2021/02/06 11:08:16 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/errno.h>
#include "minishell.h"
#include "libft/libft.h"

void	puterr_cd_getcwd(void)
{
	ft_putstr_fd("cd: error: getcwd: error: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

int		puterr_cd(char *file)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	return (1);
}

void	puterr_cd_getcwd_dot(void)
{
	ft_putstr_fd("cd: error retrieving current directory: ", 2);
	ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

int		is_path_slasla(char *path)
{
	if (ft_strncmp(path, "//", 2) == 0)
	{
		if (path[2] != '/')
			return (1);
	}
	return (0);
}

int		is_path_sla(char *path)
{
	if (ft_strncmp(path, "/", 1) == 0)
	{
		if (path[1] != '/')
			return (1);
	}
	return (0);
}
