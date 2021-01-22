/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 18:44:13 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/22 15:59:22 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/stat.h>

#include "libft/libft.h"
#include "minishell.h"

typedef struct stat	t_stat;

/*
** Globals
*/

char	**g_path;

/*
** Main Utilities
*/

void	die(char *msg)
{
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	exit(1);
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

int		is_cmd_builtins(char *cmd, char **builtins)
{
	int i;

	i = 0;
	while (builtins[i])
		if (ft_strcmp(cmd, builtins[i++]) == 0)
			return (0);
	return (42);
}

char	**set_builtins_name(void)
{
	char	**builtins;

	builtins = ft_split("time sleep wc ls echo cat cd pwd export unset env exit", ' ');
	if (!builtins)
	{
		ft_putstr_fd(strerror(errno), 2);
		exit(1);
	}
	return (builtins);
}

char	*is_cmd_exist(char **paths, char *cmd)
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

/*
** Execute
*/

pid_t	start_command(t_command *c, int *haspipe, int lastpipe[2], char *envp[])
{
	pid_t	pid;
	int		currentpipe[2];

	if (c->op == TOKEN_PIPE)
		pipe(currentpipe);
	pid = fork();
	if (pid < 0)
		die("Failed to fork");
	else if (pid == 0)
	{
		if (*haspipe)
		{
			close(lastpipe[1]);
			if (dup2(lastpipe[0], 0) == -1)
				die(strerror(errno));
			close(lastpipe[0]);
		}
		if (c->op == TOKEN_PIPE)
		{
			close(currentpipe[0]);
			if (dup2(currentpipe[1], 1) == -1)
				die(strerror(errno));
			close(currentpipe[1]);
		}

		handle_redir(c);

		if (execve(is_cmd_exist(g_path, c->argv[0]), c->argv, envp) < 0)
		{
			perror("failed to execve");
			_exit(1);
		}
		perror("This should not be printed if execvp is successful");
		_exit(1);
	}
	if (*haspipe)
	{
		close(lastpipe[0]);
		close(lastpipe[1]);
	}
	if (c->op == TOKEN_PIPE)
	{
		*haspipe = 1;
		ft_memmove(lastpipe, currentpipe, sizeof(currentpipe));
	}
	else
		*haspipe = 0;
	return (pid);
}

t_command	*do_pipeline(t_command *c, char *envp[])
{
	int			haspipe = 0;
	int			lastpipe[2] = { -1, -1 };

	while (c)
	{
		c->pid = start_command(c, &haspipe, lastpipe, envp);
		if (haspipe && c->next)
			c = c->next;
		else if (haspipe && !c->next)
			die("未実装");
		else
			break ;
	}
	return (c);
}

void	run_list(t_command *c, char *envp[])
{
	pid_t	exited_pid;
	int		status;

	while (c)
	{
		if (c->op == -1 || !c->argv || c->argv[c->argc] != NULL)
		{
			printf("Error: invalid command in list.\n");
			c = c->next;
			continue ;
		}

		char **builtins = set_builtins_name();
		if (is_cmd_builtins(c->argv[0], builtins))
		{
			c = c->next;
			continue ;
		}
		if (ft_strcmp(c->argv[0], "exit") == 0)
			exit(0);
		if (ft_strcmp(c->argv[0], "cd") == 0)
		{
			if (chdir(c->argv[1]) < 0)
				ft_putstr_fd(strerror(errno), 2);
			c = c->next;
			continue ;
		}
		c = do_pipeline(c, envp);
		g_pid = c->pid;
		exited_pid = waitpid(c->pid, &status, 0);
		assert(exited_pid == c->pid);
		while (wait(NULL) > 0);
		c = c->next;
		handle_signals();
	}
}

/*
** Parse
**
** 文字列commandlineをパースして線形リストt_commandを形成します
** リストのつなぎ目となるopはmandatoryだと';'と'|'のみです
*/

int			parse(char *commandline, t_command **c)
{
	int			type;
	char		*token;
	t_command	*current;
	t_command	*new;

	if (!(*c = command_new()))
		die("parse failed");
	current = *c;
	while ((commandline = get_next_token(commandline, &type, &token)) != NULL) {
		if (token_isop(type))
		{
			if (current->op)
				return (-1);
			current->op = type;
		}
		else
		{
			if (current->op)
			{
				if (!(new = command_new()))
					die("parse failed");
				current->next = new;
				current = new;
			}
			if (command_append_arg(current, token) < 0)
				die("parse failed");
		}
	}
	if (current->op == TOKEN_OTHER)
		die("parse failed");
	if (!current->op)
		current->op = type;
	return (0);
}

int			main(int argc, char *argv[], char *envp[])
{
	char		*commandline;
	t_command	*c;

	(void)argv;
	if (argc != 1)
		return (42);

	if (!(g_path = set_path_name(envp)))
		die(strerror(errno));
	while (42)
	{
		ft_putstr_fd("minishell>", 2);
		g_pid = 0;
		handle_signals();
		if (get_next_line(0, &commandline) < 0)
			die("gnl failed.");

		if (parse(commandline, &c) < 0)
			ft_putstr_fd("minishell: syntax error\n", 2);
		else if (c->argc)
			run_list(c, envp);

		free(commandline);
		command_lstclear(&c);
	}
	return (0);
}
