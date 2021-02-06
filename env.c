/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 12:12:16 by tayamamo          #+#    #+#             */
/*   Updated: 2021/02/05 18:07:24 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/errno.h>

#include "minishell.h"
#include "libft/libft.h"

void	env_print(char *str, int quote)
{
	char	*p;

	p = ft_strchr(str, '=');
	if (p)
	{
		write(1, str, p++ - str);
		write(1, "=", 1);
		write(1, "\"", quote);
		write(1, p, ft_strlen(p));
		write(1, "\"", quote);
		write(1, "\n", 1);
	}
	else
	{
		write(1, str, ft_strlen(str));
		write(1, "\n", 1);
	}
}

t_env	*env_get(t_env *e, char *name)
{
	while (e)
	{
		if (ft_strcmp(e->name, name) == 0)
			return (e);
		e = e->next;
	}
	return (NULL);
}

void	env_set_existing(t_env *old, t_env *new)
{
	if (new->value)
	{
		free(old->value);
		old->value = new->value;
		new->value = NULL;
	}
	env_free(new);
}

void	env_set(t_env **e, t_env *new)
{
	t_env	*tmp;

	if (e == NULL || new == NULL)
		return ;
	if (*e == NULL)
	{
		*e = new;
		return ;
	}
	tmp = *e;
	while (tmp->next)
	{
		if (ft_strcmp(tmp->name, new->name) == 0)
		{
			env_set_existing(tmp, new);
			return ;
		}
		tmp = tmp->next;
	}
	if (ft_strcmp(tmp->name, new->name) == 0)
		env_set_existing(tmp, new);
	else
		tmp->next = new;
}

void	env_unset(t_env **ep, char *name)
{
	t_env	*e;
	t_env	*prev;

	if (ep == NULL || name == NULL)
		return ;
	e = *ep;
	prev = NULL;
	while (e)
	{
		if (ft_strcmp(e->name, name) == 0)
		{
			if (prev)
				prev->next = e->next;
			else
				*ep = e->next;
			env_free(e);
			return ;
		}
		prev = e;
		e = e->next;
	}
}
