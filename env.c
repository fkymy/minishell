/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 12:12:16 by tayamamo          #+#    #+#             */
/*   Updated: 2021/01/29 21:02:59 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "minishell.h"
#include "libft/libft.h"

extern char		**environ;

void	env_update_shlvl(t_list *env)
{
	t_list	*tmp;
	int		lvl;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(((t_dict *)tmp->content)->key, "SHLVL") == 0)
		{
			lvl = ft_atoi(((t_dict *)tmp->content)->val);
			lvl++;
			free(((t_dict *)tmp->content)->val);
			((t_dict *)tmp->content)->val = ft_itoa(lvl);
			break ;
		}
		tmp = tmp->next;
	}
}

char	*env_get_key(char *env)
{
	size_t	i;

	i = 0;
	while (env[i] != '=')
		i++;
	return (ft_substr(env, 0, i));
}

t_dict	*env_make_new_dict(char *env)
{
	t_dict	*new;
	size_t	i;

	i = 0;
	while (env[i] != '=')
		i++;
	new = dict_make_new(
			ft_substr(env, 0, i),
			ft_substr(env, i + 1, ft_strlen(env)));
	return (new);
}

t_list		*env_initialize(void)
{
	t_list		*head;
	t_list		*new_list;
	size_t		i;

	i = 0;
	head = NULL;
	while (environ[i])
	{
		if (ft_strncmp(env_get_key(environ[i]), "OLDPWD", 7) == 0)
			i++;
		else
		{
			new_list = ft_lstnew(env_make_new_dict(environ[i]));
			ft_lstadd_back(&head, new_list);
			i++;
		}
	}
	if (dict_get_val(head, "PWD") == NULL)
	{
		new_list = ft_lstnew(dict_make_new("PWD", getcwd(NULL, 0)));
		ft_lstadd_back(&head, new_list);
	}
	if (dict_get_val(head, "SHLVL") == NULL)
	{
		new_list = ft_lstnew(dict_make_new("SHLVL", ft_strdup("0")));
		ft_lstadd_back(&head, new_list);
	}
	env_update_shlvl(head);
	if (dict_get_val(head, "_") == NULL)
	{
		new_list = ft_lstnew(dict_make_new("_", ft_strjoin(getcwd(NULL, 0), "./minishell")));
		ft_lstadd_back(&head, new_list);
	}
	if (dict_get_val(head, "PATH") == NULL)
	{
		new_list = ft_lstnew(dict_make_new("PATH", "/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."));
		ft_lstadd_back(&head, new_list);
	}
	return (head);
}
