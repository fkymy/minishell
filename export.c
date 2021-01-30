/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 20:08:11 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/30 19:04:29 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft/libft.h"
#include "minishell.h"

typedef struct	s_env
{
	struct s_env	*next;
	char			*name;
	char			*value;
}				t_env;

t_env	*g_env;

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

// env command
int			env_list_all(void)
{
	char	**envp;

	if (!(envp = env_make_envp(0)))
		return (1); // exit status
	while (*envp)
	{
		ft_putstr_fd(*envp, 1);
		ft_putstr_fd("\n", 1);
		envp++;
	}
	return (0);
}

void	swap_elements(char **lhs, char **rhs)
{
	char *tmp;

	tmp = *lhs;
	*lhs = *rhs;
	*rhs = tmp;
}

int		comparision(char *lhs, char *rhs)
{
	int	lchanged = 0;
	int	rchanged = 0;
	int	i = 0;
	while (lhs[i])
	{
		if (lhs[i] == '=')
		{
			lchanged = 1;
			break ;
		}
		i++;
	}
	int	j = 0;
	while (rhs[j])
	{
		if (rhs[j] == '=')
		{
			rchanged = 1;
			break ;
		}
		j++;
	}

	lhs[i] = '\0';
	rhs[j] = '\0';
	int ret = ft_strcmp(lhs, rhs);
	if (lchanged)
		lhs[i] = '=';
	if (rchanged)
		rhs[j] = '=';
	return (ret);
}

void	ft_strssort(char *array[], int size)
{
	int i;
	int j;
	int did_swap;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		did_swap = 0;
		while (j < size - 1 - i)
		{
			if (comparision(array[j], array[j + 1]) > 0)
			{
				swap_elements(&array[j], &array[j + 1]);
				did_swap = 1;
			}
			j++;
		}
		if (!did_swap)
			return ;
		i++;
	}
}

int			export_list_all(void)
{
	int		i;
	char	*p;
	char	**envp;

	int t = 0;

	envp = env_make_envp(1);
	ft_strssort(envp, ft_strslen(envp));
	i = 0;
	while(envp[i])
	{
		if (envp[i][0] == '_' && envp[i][1] == '=')
		{
			i++;
			continue ;
		}
		ft_putstr_fd("declare -x ", 1);
		p = strchr(envp[i], '=');
		if (p)
		{
			write(1, envp[i], p++ - envp[i]);
			write(1, "=\"", 2);
			write(1, p, ft_strlen(p));
			write(1, "\"\n", 2);
		}
		else
		{
			write(1, envp[i], ft_strlen(envp[i]));
			write(1, "\n", 1);
		}
		i++;
	}
	t = 0;
	while (envp[t])
		free(envp[t++]);
	free(envp);
	return (0);
}

int			isvalid(char *str)
{
	int	i;

	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '=')
			return (0);
		i++;
	}
	return (1);
}

int			export(int ispipe, int haspipe, char *argv[], char *envp[])
{
	(void)haspipe;
	(void)ispipe;
	(void)envp;

	int error = 0;

	++argv;
	if (!*argv)
	{
		return (export_list_all());
	}

	// export _A _B= _C=ccc =bad 9=bad
	while (*argv)
	{
		if (!isvalid(*argv))
		{
			ft_putstr_fd("minishell: export: not a valid identifier", 2);
			// set exit status to 1
			error = 1;
			argv++;
			continue ;
		}

		// get name (until \0 or =)
		// if has =
		// get word (empty or some word)
		//
		// if name exist
		//   if word is not NULL, set name=word to same environ[i]
		//   else do nothion
		// else
		//   set name[=[word]] to new environ[i]

		argv++;
	}
	return (error ? 1 : 0);
}
