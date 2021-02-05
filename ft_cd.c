/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 17:26:18 by tayamamo          #+#    #+#             */
/*   Updated: 2021/02/06 00:43:44 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/errno.h>
#include "minishell.h"
#include "libft/libft.h"

static char	*get_cwd(char *argv[])
{
	char	*cwd;
	char	*tmp;

	if ((cwd = getcwd(NULL, 0)) == NULL)
	{
		if (argv[1] == NULL)
			puterr_cd_getcwd();
		else
		{
			puterr_cd_getcwd_dot();
			tmp = g_pwd;
			if ((g_pwd = ft_strjoin_chr(g_pwd, argv[1], '/')) == NULL)
				die(strerror(errno));
			free(tmp);
			env_set(&g_env, env_make_new("OLDPWD", get_pwd()));
			env_set(&g_env, env_make_new("PWD", g_pwd));
		}
	}
	return (cwd);
}

static int	home_value_is_null(t_env *home)
{
	char	*cwd;

	if ((cwd = getcwd(NULL, 0)) == NULL)
	{
		if (chdir(home->value) == 0)
			puterr_cd_getcwd();
		else
			puterr_cd(home->value);
		return (1);
	}
	free(cwd);
	env_set(&g_env, env_make_new("OLDPWD", get_pwd()));
	env_set(&g_env, env_make_new("PWD", g_pwd));
	return (0);
}

static int	argv1_is_null(void)
{
	t_env	*home;

	home = env_get(g_env, "HOME");
	if (home == NULL)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	if (home->value == NULL || home->value[0] == '\0')
		return (home_value_is_null(home));
	if (chdir(home->value) < 0)
	{
		puterr_cd(home->value);
		return (1);
	}
	env_set(&g_env, env_make_new("OLDPWD", get_pwd()));
	env_set(&g_env, env_make_new("PWD", home->value));
	free(g_pwd);
	g_pwd = ft_strdup(home->value);
	return (0);
}

static int	argv1_is_null_char(void)
{
	char	*cwd;

	if ((cwd = getcwd(NULL, 0)) == NULL)
		return (puterr_cd(""));
	free(cwd);
	env_set(&g_env, env_make_new("OLDPWD", get_pwd()));
	env_set(&g_env, env_make_new("PWD", g_pwd));
	return (0);
}

int			ft_cd(char *argv[])
{
	char	*cwd;

	if (argv[1] == NULL)
		return (argv1_is_null());
	if (ft_strcmp(argv[1], "") == 0)
		return (argv1_is_null_char());
	if (chdir(argv[1]) < 0)
		return (puterr_cd(argv[1]));
	if ((cwd = get_cwd(argv)) == NULL)
		return (1);
	free(cwd);
	if (ft_strncmp(argv[1], "/", 1) == 0)
	{
		free(g_pwd);
		g_pwd = format_pwd(ft_strdup(argv[1]), ft_strdup(""));
	}
	else
		g_pwd = format_pwd(g_pwd, ft_strdup(argv[1]));
	env_set(&g_env, env_make_new("OLDPWD", get_pwd()));
	env_set(&g_env, env_make_new("PWD", g_pwd));
	return (0);
}
