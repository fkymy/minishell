/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 18:02:17 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/05 18:03:35 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

char	*env_split_name(char *str)
{
	char	*name;
	size_t	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (!(name = ft_substr(str, 0, i)))
		die(strerror(errno));
	return (name);
}

char	*env_split_value(char *str)
{
	char	*value;
	size_t	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] == '=')
	{
		if (!(value = ft_strdup(str + i + 1)))
			die(strerror(errno));
	}
	else
		value = NULL;
	return (value);
}

char	*env_join_name_value(t_env *env)
{
	char	*str;

	if (!env->name)
		return (NULL);
	if (env->value)
		str = ft_strjoin_chr(env->name, env->value, '=');
	else
		str = ft_strdup(env->name);
	return (str);
}

int		env_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		env = env->next;
		size++;
	}
	return (size);
}

void	env_free(t_env *e)
{
	free(e->name);
	free(e->value);
	free(e);
}
