/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 13:16:00 by tayamamo          #+#    #+#             */
/*   Updated: 2021/02/08 23:52:16 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/errno.h>
#include "minishell.h"
#include "libft/libft.h"

char	*set_path(char *argv)
{
	char	*path;

	if (ft_strncmp(argv, "/", 1) == 0)
		path = format_pwd(ft_strdup(argv), ft_strdup(""));
	else
		path = format_pwd(ft_strdup(g_pwd), ft_strdup(argv));
	return (path);
}

void	check_symbolic_link(char *argv)
{
	char *cwd;

	cwd = set_path(argv);
	if (chdir(cwd) < 0)
	{
		free(g_pwd);
		g_pwd = getcwd(NULL, 0);
	}
	free(cwd);
}
