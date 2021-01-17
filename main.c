/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 18:44:13 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/17 12:06:00 by tayamamo         ###   ########.fr       */
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
#include "token.h"
#include "command.h"

typedef struct stat	t_stat;

/*
** Globals
*/

char	**g_path;

/*
** Main Utilities
*/

#ifdef DEBUG
void	debug(const char *fmt, ...)
{
	va_list	params;

	va_start(params, fmt);
	printf("[%d] ", getpid());
	vprintf(fmt, params);
	printf("\n");
	va_end(params);
}
#else
void	debug(const char *fmt, ...)
{
	(void)fmt;
}
#endif

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

	builtins = ft_split("wc ls echo cat cd pwd export unset env exit", ' ');
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

void	run_pipe(t_command *c, char *envp[])
{
	int		pipefd[2];
	pid_t	child;
	int		status;

	if (c->op != TOKEN_PIPE)
	{
		return ;
	}
	if (pipe(pipefd) == -1)
		die(strerror(errno));
	child = fork();
	if (child == -1)
		die(strerror(errno));
	else if (child == 0)
	{
		close(pipefd[0]); // 読み込み先をクローズ
		if (dup2(pipefd[1], 1) == -1) // 書き込み先を標準出力に
			die(strerror(errno));
		/* close(1); */
		close(pipefd[1]); // 書き込み先をクローズ
		if (execve(is_cmd_exist(g_path, c->argv[0]), c->argv, envp) == -1)
			die(strerror(errno));
	}
	else
	{
		close(pipefd[1]); // 書き込み先をクローズ
		if (dup2(pipefd[0], 0) == -1) // 読み込み先を標準入力へ
			die(strerror(errno));
		/* close(0); */
		close(pipefd[0]); // 読み込み先をクローズ
		if (waitpid(child, &status, 0) == -1)
			die(strerror(errno));
		c = c->next;
		run_pipe(c, envp);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	return ;
}

/*
** Execute
*/

pid_t	start_command(t_command *c, char *envp[])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		die("Failed to fork");
	else if (pid == 0)
	{
		if (execve(is_cmd_exist(g_path, c->argv[0]), c->argv, envp) < 0)
		{
			perror("failed to execve");
			_exit(1);
		}
		perror("This should not be printed if execvp is successful");
		_exit(1);
	}
	c->pid = pid;
	return (c->pid);
}

void	run_list(t_command *c, char *envp[])
{
	pid_t 	command_pid;
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

		if (c->op == TOKEN_PIPE)
		{
			run_pipe(c, envp);
		}
		else
		{
			command_pid = start_command(c, envp);
			exited_pid = waitpid(command_pid, &status, 0);
			assert(exited_pid == c->pid);
			if (WIFEXITED(status))
				debug("child with pid %d exited with status %d", exited_pid, WEXITSTATUS(status));
			else
				debug("child exited abnormally with status: %d", status);
		}
		c = c->next;
	}
}

/*
** Parse
**
** 文字列commandlineをパースして線形リストt_commandを形成します
** リストのつなぎ目となるopはmandatoryだと';'と'|'のみです
*/

t_command	*parse(char *commandline)
{
	int			type;
	char		*token;
	t_command	*head;
	t_command	*c;

	c = command_new();
	head = c;
	while ((commandline = get_next_token(commandline, &type, &token)) != NULL) {
		if (token_isop(type))
		{
			if (c->op)
			{
				printf("minishell: syntax error near unexpected token %s\n", token);
				return (head);
			}
			c->op = type;
		}
		else
		{
			if (c->op)
			{
				t_command *new = command_new();
				c->next = new;
				c = new;
			}
			command_append_arg(c, token);
		}
	}
	if (!c->op)
		c->op = type;
	return (head);
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
		if (get_next_line(0, &commandline) == -1)
			die("gnl failed.\n");

		c = parse(commandline);
		if (c->argc)
			run_list(c, envp);

		free(commandline);
		command_lstclear(&c);
	}
	return (0);
}
