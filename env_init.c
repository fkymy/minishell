/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 17:55:39 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/05 20:16:05 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

void		env_update_shlvl(t_env *e)
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

void		init_necessary_env(t_env **head)
{
	char	*cwd;
	char	*shell;

	if (!(cwd = getcwd(NULL, 0)))
		die(strerror(errno));
	if (!(shell = ft_strjoin_chr(cwd, "minishell", '/')))
		die(strerror(errno));
	if (env_get(*head, "PATH") == NULL)
		env_set(head, env_make_new("PATH",
					"/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."));
	env_set(head, env_make_new("OLDPWD", NULL));
	if (env_get(*head, "PWD") == NULL)
		env_set(head, env_make_new("PWD", cwd));
	if (env_get(*head, "SHLVL") == NULL)
		env_set(head, env_make_new("SHLVL", "0"));
	env_update_shlvl(*head);
	if (env_get(*head, "_") == NULL)
		env_set(head, env_make_new("_", shell));
	free(cwd);
	free(shell);
}

t_env		*env_init(void)
{
	extern char	**environ;
	t_env		*head;
	size_t		i;
	char		*name;

	i = 0;
	head = NULL;
	while (environ[i])
	{
		name = env_split_name(environ[i]);
		if (ft_strcmp(name, "OLDPWD") == 0)
			i++;
		else
			env_set(&head, env_new(environ[i++]));
		free(name);
	}
	init_necessary_env(&head);
	return (head);
}
