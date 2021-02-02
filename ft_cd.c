/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 17:26:18 by tayamamo          #+#    #+#             */
/*   Updated: 2021/02/02 19:30:08 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include "minishell.h"
#include "libft/libft.h"

void	putstr_cd_home(char *prompt, char *cmd)
{
	ft_putstr_fd(prompt, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd("HOME not set", 2);
	ft_putstr_fd("\n", 2);
}

void	putstr_cd(char *prompt, char *cmd, char *file)
{
	ft_putstr_fd(prompt, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

int		ft_cd(char *argv[])
{
	t_env			*home;
	extern t_env	*g_env;
	char			*cwd;

	if (!(cwd = getcwd(NULL, 0)))
		die(strerror(errno));
	if (argv[1] == NULL)
	{
		home = env_get(g_env, "HOME");
		if (home == NULL || chdir(home->value) < 0)
		{
			putstr_cd_home("minihsell", argv[0]);
			free(cwd);
			return (1);
		}
		env_set(&g_env, env_make_new("PWD", home->value));
		env_set(&g_env, env_make_new("OLDPWD", cwd));
		free(cwd);
		return (0);
	}
	else if (ft_strcmp(argv[1], "") == 0)
	{
		env_set(&g_env, env_make_new("OLDPWD", cwd));
		free(cwd);
		return (0);
	}
	else if (chdir(argv[1]) < 0)
	{
		putstr_cd("minihsell", argv[0], argv[1]);
		free(cwd);
		return (1);
	}
	env_set(&g_env, env_make_new("OLDPWD", cwd));
	free(cwd);
	if (!(cwd = getcwd(NULL, 0)))
		die(strerror(errno));
	env_set(&g_env, env_make_new("PWD", cwd));
	free(cwd);
	return (0);
}
