/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 18:44:13 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/12 05:41:01 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <stdio.h>

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

char	**set_path_name(char *envp[])
{
	int		i;
	char	*tmp;
	char	**path;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH", 4))
		i++;
	path = NULL;
	if (envp[i])
	{
		if (!(tmp = ft_substr(envp[i], 5, ft_strlen(envp[i]))))
			return (NULL);
		if (!(path = ft_split(tmp, ':')))
			return (NULL);
	}
	return (path);
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

char
	*is_cmd_exist(char **paths, char *cmd)
{
	int		i;
	t_stat	stat_buf;
	char	*tmp;
	char	*path2cmd;

	i = 0;
	while (42)
	{
		if (!(tmp = ft_strjoin(paths[i], "/")))
			break ;
		if (!(path2cmd = ft_strjoin(tmp, cmd)))
			break ;
		ft_free_null(&tmp);
		if (!stat(path2cmd, &stat_buf))
			return (path2cmd);
		ft_free_null(&path2cmd);
		i++;
	}
	ft_free_null(&tmp);
	ft_free_null(&path2cmd);
	return (NULL);
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
	char		**path;

	(void)argc;
	(void)argv;
	ft_putstr_fd("よろしくお願いします！\n", 1);
	if (argc != 1)
		return (42);
	builtins = set_builtins_name();
	if (!(path = set_path_name(envp)))
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		exit(1);
	}
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
		cmd = is_cmd_exist(path, cmds[0]);
		if (child_pid == 0 && cmd)
		{
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
