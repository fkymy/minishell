/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_make.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 18:05:04 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/05 18:06:02 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/errno.h>
#include "libft/libft.h"
#include "minishell.h"

char	**env_make_envp(t_env *e, int isexport)
{
	char	**envp;
	int		i;
	t_env	*current;

	if (!(envp = malloc(sizeof(char *) * (env_size(e) + 1))))
		return (NULL);
	i = 0;
	current = e;
	while (current)
	{
		if (ft_strcmp(current->name, "_") == 0)
		{
			current = current->next;
			continue ;
		}
		if (current->value || isexport)
			if (!(envp[i++] = env_join_name_value(current)))
				return (ft_teardown(envp, i));
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}

t_env	*env_make_new(char *name, char *value)
{
	t_env	*new;

	if (!name)
		return (NULL);
	if ((new = malloc(sizeof(t_env))) == NULL)
		die("malloc failed\n");
	new->next = NULL;
	if ((new->name = ft_strdup(name)) == NULL)
		die(strerror(errno));
	if (value)
	{
		if ((new->value = ft_strdup(value)) == NULL)
			die(strerror(errno));
	}
	else
		new->value = NULL;
	return (new);
}

t_env	*env_new(char *str)
{
	t_env	*new;
	char	*name;
	char	*value;

	if (!str || !*str)
		return (NULL);
	name = env_split_name(str);
	value = env_split_value(str);
	new = env_make_new(name, value);
	free(name);
	free(value);
	return (new);
}
