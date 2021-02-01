/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 15:32:02 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/01 16:08:19 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

static int	env_isvalid(char *str)
{
	int	i;

	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int			ft_unset(char *argv[])
{
	int	error;
	int	i;

	if (!argv[1])
		return (0);
	error = 0;
	i = 1;
	while (argv[i])
	{
		if (!env_isvalid(argv[i]))
		{
			ft_putstr_fd("minishell: unset: not a valid identifier\n", 2);
			g_exit_status = 1;
			error = 1;
			i++;
			continue ;
		}
		env_unset(&g_env, argv[i]);
		i++;
	}
	return (error ? 1 : 21);
}
