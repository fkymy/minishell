/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 15:32:29 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/04 16:05:20 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft/libft.h"
#include "minishell.h"

int			env_list_all(void)
{
	char	**envp;

	if (!(envp = env_make_envp(g_env, 0)))
		return (1);
	while (*envp)
	{
		ft_putstr_fd(*envp, 1);
		ft_putstr_fd("\n", 1);
		envp++;
	}
	return (0);
}

static int	compare(char *lhs, char *rhs)
{
	int		ret;
	char	*lname;
	char	*rname;

	lname = env_split_name(lhs);
	rname = env_split_name(rhs);
	ret = ft_strcmp(lname, rname);
	free(lname);
	free(rname);
	return (ret);
}

static int	export_list(t_env *env)
{
	int		i;
	char	**envp;

	envp = env_make_envp(env, 1);
	ft_strsort(envp, ft_strslen(envp), compare);
	i = 0;
	while (envp[i])
	{
		write(1, "declare -x ", 11);
		env_print(envp[i], 1);
		i++;
	}
	ft_split_free_null(envp);
	return (0);
}

int			env_isvalid(char *str)
{
	int	i;

	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int			ft_export(char *argv[])
{
	int	i;
	int	error;

	if (!argv[1])
		return (export_list(g_env));
	error = 0;
	i = 1;
	while (argv[i])
	{
		if (!env_isvalid(argv[i]))
		{
			ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
			error = 1;
			i++;
			continue ;
		}
		env_set(&g_env, env_new(argv[i]));
		i++;
	}
	return (error ? 1 : 0);
}
