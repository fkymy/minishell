/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 12:12:16 by tayamamo          #+#    #+#             */
/*   Updated: 2021/01/30 20:33:43 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft/libft.h"

extern char		**environ;
extern t_env	*g_env;

int			env_size(t_env *env)
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

char		*ft_strjoin_chr(char *s1, char *s2, char c)
{
	char	*res;
	int		size;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	size = ft_strlen(s1) + ft_strlen(s2) + 1;
	if (!(res = malloc(sizeof(char) * (size + 1))))
		return (NULL);
	i = 0;
	j = 0;
	while (s1[j])
		res[i++] = s1[j++];
	res[i++] = c;
	j = 0;
	while (s2[j])
		res[i++] = s2[j++];
	res[i] = '\0';
	return (res);
}

// utility
static char	**teardown(char **res, int allocations)
{
	int i;

	i = 0;
	while (i < allocations)
		free(res[i]);
	free(res);
	return (NULL);
}

char		*env_join_name_value(t_env *env)
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

char		**env_make_envp(int isexport)
{
	char	**envp;
	int		i;
	t_env	*current;

	if (!(envp = malloc(sizeof(char *) * (env_size(g_env) + 1))))
		return (NULL);
	i = 0;
	current = g_env;
	while (current)
	{
		if (current->value || isexport)
			if (!(envp[i++] = env_join_name_value(current)))
				return (teardown(envp, i));
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}

char	*env_get_key(char *env)
{
	size_t	i;

	i = 0;
	while (env[i] != '=')
		i++;
	return (ft_substr(env, 0, i));
}

char	*env_get_value(t_env *e, char *name)
{
	t_env	*current;

	current = e;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	env_append(t_env **e, t_env *new)
{
	t_env	*tmp;

	if (e == NULL || new == NULL)
		return ;
	if (*e == NULL)
		*e = new;
	else
	{
		tmp = *e;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_env	*env_make_new(char *name, char *value)
{
	t_env	*new;

	if ((new = malloc(sizeof(t_env))) == NULL)
		die("malloc failed\n");
	new->next = NULL;
	new->name = name;
	new->value = value;
	return (new);
}

t_env	*env_new(char *e)
{
	t_env	*new;
	size_t	i;

	i = 0;
	while (e[i] != '=')
		i++;
	new = env_make_new(ft_substr(e, 0, i), ft_substr(e, i + 1, ft_strlen(e)));
	return (new);
}

void	env_update_shlvl(t_env *e)
{
	t_env	*current;
	int		shlvl;

	current = e;
	while (current)
	{
		if (ft_strcmp(current->name, "SHLVL") == 0)
		{
			shlvl = ft_atoi(current->value);
			shlvl++;
			free(current->value);
			current->value = ft_itoa(shlvl);
			break ;
		}
		current = current->next;
	}
}

t_env	*env_init(void)
{
	t_env	*head;
	size_t	i;

	i = 0;
	head = NULL;
	while (environ[i])
	{
		if (ft_strncmp(env_get_key(environ[i]), "OLDPWD", 7) == 0)
			i++;
		else
			env_append(&head, env_new(environ[i++]));
	}
	if (env_get_value(head, "PATH") == NULL)
		env_append(&head, env_make_new("PATH", "/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."));
	if (env_get_value(head, "PWD") == NULL)
		env_append(&head, env_make_new("PWD", getcwd(NULL, 0)));
	if (env_get_value(head, "SHLVL") == NULL)
		env_append(&head, env_make_new("SHLVL", ft_strdup("0")));
	env_update_shlvl(head);
	if (env_get_value(head, "_") == NULL)
		env_append(&head, env_make_new("_", ft_strjoin(getcwd(NULL, 0), "/./minishell")));
	return (head);
}
