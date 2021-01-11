/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 18:44:13 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/12 03:21:23 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int		is_cmd_builtins(char *cmd, char **builtins)
{
	int i;

	i = 0;
	while (builtins[i])
	{
		if (!ft_strncmp(cmd, builtins[i], ft_strlen(cmd)))
			if (!ft_strncmp(builtins[i], cmd, ft_strlen(builtins[i])))
				return (0);
		i++;
	}
	return (42);
}

char	**set_builtins_name()
{
	char	**builtins;

	builtins = ft_split("ls echo cat cd pwd export unset env exit", ' ');
	if (!builtins)
	{
		ft_putstr_fd(strerror(errno), 2);
		exit(1);
	}
	return (builtins);
}

int	main(int argc, char *argv[], char *envp[])
{
	char		**cmds;
	char		*cmd;
	char		**line;
	pid_t		child_pid;
	int			stat_loc;
	int			ret;
	char		**builtins;

	(void)argc;
	(void)argv;
	ft_putstr_fd("よろしくお願いします！\n", 1);
	if (argc != 1)
		return (42);
	builtins = set_builtins_name();
	while (42)
	{
		ft_putstr_fd("minishell>", 1);
		if (!(line = malloc(sizeof(char *))))
		{
			ft_putstr_fd(strerror(errno), 2);
			ft_putstr_fd("\n", 2);
			exit(1);
		}
		if ((ret = get_next_line(0, line)) == -1)
		{
			ft_putstr_fd("gnl failed.\n", 2);
			exit(1);
		}
		cmds = ft_split(*line, ' ');
		if (is_cmd_builtins(cmds[0], builtins))
			continue ;
		if (!ft_strncmp(cmds[0], "exit", ft_strlen(cmds[0])))
			break ;
		if (!ft_strncmp(cmds[0], "cd", ft_strlen(cmds[0])))
		{
			if (chdir(cmds[1]) < 0)
				ft_putstr_fd(strerror(errno), 2);
			continue ;
		}
		child_pid = fork();
		if (child_pid < 0)
			exit(1);
		if (child_pid == 0)
		{
			if (!(cmd = ft_calloc(1, sizeof(char) * (ft_strlen(cmds[0]) + 6))))
				break ;
			ft_strlcpy(cmd, "/bin/", ft_strlen(cmds[0]) + 6);
			ft_strlcat(cmd, cmds[0], ft_strlen(cmds[0]) + 6);
			if ((ret = execve(cmd, cmds, envp)) == -1)
			{
				ft_putstr_fd(strerror(errno), 2);
				ft_putstr_fd("\n", 2);
				exit(1);
			}
		}
		else
		{
			waitpid(child_pid, &stat_loc, WUNTRACED);
		}
		ft_free_null(line);
		free(line);
		ft_free_null(cmds);
		free(cmds);
		ft_free_null(&cmd);
		free(cmd);
	}
	ft_free_null(line);
	free(line);
	ft_free_null(cmds);
	free(cmds);
	ft_free_null(&cmd);
	free(cmd);
	return (0);
}
