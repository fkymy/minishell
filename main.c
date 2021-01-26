/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 18:44:13 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/26 21:26:59 by tayamamo         ###   ########.fr       */
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

int		g_exit_status = 0;
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

char	**set_path_name(void)
{
	int		i;
	char	*tmp;
	char	**path;
	extern char	**environ;

	i = 0;
	while (environ[i] && ft_strncmp(environ[i], "PATH", 4))
		i++;
	path = NULL;
	if (environ[i])
	{
		if (!(tmp = ft_substr(environ[i], 5, ft_strlen(environ[i]))))
			return (NULL);
		if (!(path = ft_split(tmp, ':')))
			return (NULL);
	}
	return (path);
}

void	free_split(char **ptr)
{
	int	i;

	if (!ptr)
		return ;
	i = 0;
	while (ptr[i])
	{
		ft_free_null(&ptr[i]);
		i++;
	}
	free(ptr);
	return ;
}

int		is_cmd_builtins(char *cmd)
{
	int i;
	char **builtins;

	builtins = ft_split("echo cd pwd export unset env exit", ' ');
	if (!builtins)
		die(strerror(errno));
	i = 0;
	while (builtins[i])
		if (ft_strcmp(cmd, builtins[i++]) == 0)
		{
			free_split(builtins);
			return (0);
		}
	free_split(builtins);
	return (42);
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

char	**process_words(char *argv[])
{
	t_wordexp	w;
	size_t		i;

	w.wordc = 0;
	w.wordv = NULL;

	i = 0;
	argv = handle_redir(argv);
	while (argv[i])
	{
		w.offset = 0;
		if (wordexp(argv[i], &w) < 0)
			die("wordexp failed");
		i++;
	}
	command_clear_args(argv);
	return (w.wordv);
}

pid_t	start_command(char *argv[], int ispipe, int haspipe, int lastpipe[2])
{
	extern char	**environ;
	pid_t		pid;
	int			newpipe[2];

	if (ispipe)
		pipe(newpipe);
	pid = fork();
	if (pid < 0)
		die("Failed to fork");
	else if (pid == 0)
	{
		if (haspipe)
		{
			close(lastpipe[1]);
			if (dup2(lastpipe[0], 0) == -1)
				die(strerror(errno));
			close(lastpipe[0]);
		}
		if (ispipe)
		{
			close(newpipe[0]);
			if (dup2(newpipe[1], 1) == -1)
				die(strerror(errno));
			close(newpipe[1]);
		}

		argv = process_words(argv);
		if (execve(is_cmd_exist(g_path, argv[0]), argv, environ) < 0)
		{
			perror("failed to execve");
			_exit(1);
		}
		perror("This should not be printed if execvp is successful");
		_exit(1);
	}
	if (haspipe)
	{
		close(lastpipe[0]);
		close(lastpipe[1]);
	}
	if (ispipe)
	{
		lastpipe[0] = newpipe[0];
		lastpipe[1] = newpipe[1];
	}
	return (pid);
}

t_command	*do_pipeline(t_command *c)
{
	int	ispipe = 0;
	int	haspipe = 0;
	int	lastpipe[2] = { -1, -1 };

	while (c)
	{
		ispipe = c->op == TOKEN_PIPE ? 1 : 0;
		c->pid = start_command(c->argv, ispipe, haspipe, lastpipe);
		haspipe = ispipe;
		if (ispipe && c->next)
			c = c->next;
		else if (ispipe && !c->next)
			die("未実装");
		else
			break ;
	}
	return (c);
}

void	run_list(t_command *c)
{
	pid_t	exited_pid;
	int		status;

	while (c)
	{
		if (c->op == -1 || !c->argv || c->argv[c->argc] != NULL)
		{
			ft_putstr_fd("Error: invalid command in list.\n", 2);
			c = c->next;
			continue ;
		}

		if (!is_cmd_exist(g_path, c->argv[0])
				&& is_cmd_builtins(c->argv[0]))
		{
			ft_putstr_fd("command not found\n", 2);
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
		c = do_pipeline(c);
		exited_pid = waitpid(c->pid, &status, 0);
		assert(exited_pid == c->pid);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else
			die("child exited abnormally");
		while (wait(NULL) > 0);
		c = c->next;
	}
}

int			main(int argc, char *argv[], char *envp[])
{
	char		*commandline;
	t_command	*c;

	(void)argv;
	(void)envp;
	if (argc != 1)
		return (42);

	if (!(g_path = set_path_name()))
		die(strerror(errno));
	while (42)
	{
		ft_putstr_fd("minishell>", 2);
		if (get_next_line(0, &commandline) < 0)
			die("gnl failed.");

		if (parse(commandline, &c) < 0)
			ft_putstr_fd("minishell: syntax error\n", 2);
		else if (c->argc)
			run_list(c);

		free(commandline);
		command_lstclear(&c);
	}
	return (0);
}
