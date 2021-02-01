/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 12:12:16 by tayamamo          #+#    #+#             */
/*   Updated: 2021/02/01 14:57:15 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/errno.h>
#include "minishell.h"
#include "libft/libft.h"

extern char		**environ;

int		ft_env(char *argv[], char **err)
{
	t_env	*e;
	char	*line;

	if (ft_strslen(argv) != 1)
	{
		ft_stradd(err, "minishell: env: too many arguments\n");
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

void	env_print(char *str, int quote)
{
	char	*p;

	p = strchr(str, '=');
	if (p)
	{
		write(1, str, p++ - str);
		write(1, "=", 2);
		write(1, "\"", quote);
		write(1, p, ft_strlen(p));
		write(1, "\"", quote);
		write(1, "\n", 2);
	}
	else
	{
		write(1, str, ft_strlen(str));
		write(1, "\n", 1);
	}
}

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

char		**env_make_envp(t_env *e, int isexport)
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

void	env_free(t_env *e)
{
	free(e->name);
	free(e->value);
	free(e);
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
				*ep = NULL;
			env_free(e);
			return ;
		}
		prev = e;
		e = e->next;
	}
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
	char	*cwd;
	char	*shell;

	i = 0;
	head = NULL;
	while (environ[i])
	{
		if (ft_strcmp(env_split_name(environ[i]), "OLDPWD") == 0)
			i++;
		else
			env_set(&head, env_new(environ[i++]));
	}
	if (!(cwd = getcwd(NULL, 0)))
		die(strerror(errno));
	if (!(shell = ft_strjoin_chr(cwd, "./minishell", '/')))
		die(strerror(errno));
	if (env_get(head, "PATH") == NULL)
		env_set(&head, env_make_new("PATH", "/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."));
	if (env_get(head, "PWD") == NULL)
		env_set(&head, env_make_new("PWD", cwd));
	if (env_get(head, "SHLVL") == NULL)
		env_set(&head, env_make_new("SHLVL", "0"));
	env_update_shlvl(head);
	if (env_get(head, "_") == NULL)
		env_set(&head, env_make_new("_", shell));
	free(cwd);
	free(shell);
	return (head);
}
