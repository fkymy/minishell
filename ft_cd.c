/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 17:26:18 by tayamamo          #+#    #+#             */
/*   Updated: 2021/02/04 12:39:37 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/errno.h>
#include "minishell.h"
#include "libft/libft.h"

static char	*get_cwd(void)
{
	char	*cwd;

	if ((cwd = getcwd(NULL, 0)) == NULL)
	{
		ft_putstr_fd("cd: error: getcwd: error: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	return (cwd);
}

static void	putstr_cd(char *file)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

static int	home_is_null(char *cwd)
{
	ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	free(cwd);
	return (1);
}

static int	argv1_is_null(char *cwd)
{
	t_env			*home;

	home = env_get(g_env, "HOME");
	if (home == NULL)
		return (home_is_null(cwd));
	else if (home->value == NULL || home->value[0] == '\0')
	{
		env_set(&g_env, env_make_new("OLDPWD", get_pwd()));
		env_set(&g_env, env_make_new("PWD", cwd));
		free(cwd);
		return (0);
	}
	else if (chdir(home->value) < 0)
	{
		putstr_cd(home->value);
		free(cwd);
		return (1);
	}
	env_set(&g_env, env_make_new("OLDPWD", get_pwd()));
	env_set(&g_env, env_make_new("PWD", home->value));
	free(cwd);
	return (0);
}

int			ft_cd(char *argv[])
{
	char			*cwd;

	cwd = get_cwd();
	if (argv[1] == NULL)
		return (argv1_is_null(cwd));
	else if (ft_strcmp(argv[1], "") == 0)
	{
		env_set(&g_env, env_make_new("OLDPWD", get_pwd()));
		env_set(&g_env, env_make_new("PWD", cwd));
		free(cwd);
		return (0);
	}
	else if (chdir(argv[1]) < 0)
	{
		putstr_cd(argv[1]);
		free(cwd);
		return (1);
	}
	env_set(&g_env, env_make_new("OLDPWD", get_pwd()));
	free(cwd);
	if ((cwd = get_cwd()) == NULL)
		cwd = ft_strjoin_chr(get_pwd(), argv[1], '/');
	env_set(&g_env, env_make_new("PWD", cwd));
	free(cwd);
	return (0);
}
