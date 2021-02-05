/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 18:01:05 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/05 18:01:29 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int	ft_env(char *argv[])
{
	t_env	*e;
	char	*line;

	if (ft_strslen(argv) != 1)
	{
		ft_putstr_fd("minishell: env: too many arguments\n", 2);
		return (1);
	}
	e = g_env;
	while (e)
	{
		if (e->value)
		{
			line = env_join_name_value(e);
			env_print(line, 0);
			free(line);
		}
		e = e->next;
	}
	return (0);
}
