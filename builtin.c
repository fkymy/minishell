/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 16:54:40 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/05 16:55:03 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int			is_builtin(char *word, int expand)
{
	int			ret;
	char		**exp;
	int			i;
	const char	*builtins[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit",
		NULL
	};

	ret = 0;
	exp = wordexp_wrap(word);
	if (exp == NULL)
		return (ret);
	i = -1;
	while (builtins[++i])
		if (ft_strcmp(builtins[i], expand ? *exp : word) == 0)
		{
			ret = 1;
			break ;
		}
	ft_split_free_null(exp);
	return (ret);
}

int			exec_builtin(char *argv[])
{
	if (ft_strcmp(argv[0], "echo") == 0)
		return (ft_echo(argv));
	if (ft_strcmp(argv[0], "cd") == 0)
		return (ft_cd(argv));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(argv[0], "export") == 0)
		return (ft_export(argv));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (ft_unset(argv));
	if (ft_strcmp(argv[0], "env") == 0)
		return (ft_env(argv));
	if (ft_strcmp(argv[0], "exit") == 0)
		return (ft_exit(argv));
	return (-1);
}

int			exec_builtin_parent(t_command *c)
{
	int	ret;
	int	in;
	int	out;

	in = -1;
	out = -1;
	ret = 0;
	if (!(c->argv = process_redir(c->argv, &in, &out)))
		ret = 1;
	else if (!(c->argv = process_words(c->argv)))
		die("failed to process words");
	else
		ret = exec_builtin(c->argv);
	if (in != -1)
		dup2(in, 0);
	if (out != -1)
		dup2(out, 1);
	return (ret);
}
