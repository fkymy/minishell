/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 18:44:13 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/03 16:22:03 by tayamamo         ###   ########.fr       */
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

int		g_exit_status;
char	**g_path;
t_env	*g_env;

/*
** Main Utilities
*/

void	die(char *msg)
{
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	exit(1);
}

int		is_cmd_builtins(char *cmd)
{
	int i;
	char **builtins;

	builtins = ft_split("export", ' ');
	if (!builtins)
		die(strerror(errno));
	i = 0;
	while (builtins[i])
		if (ft_strcmp(cmd, builtins[i++]) == 0)
		{
			ft_split_free_null(builtins);
			return (0);
		}
	ft_split_free_null(builtins);
	return (42);
}

int		is_cmd_dir(char *path2cmd)
{
	t_stat	stat_buf;

	if (stat(path2cmd, &stat_buf) == 0)
		if ((stat_buf.st_mode & S_IFDIR) == S_IFDIR)
			return (1);
	return (0);
}

int		is_cmd_permitted(char *path2cmd)
{
	t_stat	stat_buf;

	if (stat(path2cmd, &stat_buf) == 0)
		if ((stat_buf.st_mode & S_IXUSR) == S_IXUSR)
			return (1);
	return (0);
}

char	*build_absolute_path(char *cmd)
{
	char	*cwd;
	char	*path;

	if (*cmd == '/')
		return (ft_strdup(cmd));
	if (!(cwd = getcwd(NULL, 0)))
		die(strerror(errno));
	if (!(path = ft_strjoin_chr(cwd, cmd, '/')))
		die(strerror(errno));
	free(cwd);
	return (path);
}

char	*check_path(char *cmd, char *path2cmd)
{
	t_stat	stat_buf;

	if (is_cmd_dir(path2cmd) == 1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": is a directory\n", 2);
		exit(126);
	}
	if (stat(path2cmd, &stat_buf) == 0)
	{
		if (is_cmd_permitted(path2cmd) == 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(126);
		}
		return (path2cmd);
	}
	return (NULL);
}

char	*find_from_env(char *cmd, char **paths)
{
	char	*path2cmd;
	int		i;

	i = 0;
	while (paths[i])
	{
		if ((path2cmd = ft_strjoin_chr(paths[i], cmd, '/')) == NULL)
			die(strerror(errno));
		if (check_path(cmd, path2cmd))
			return (path2cmd);
		ft_free_null(&path2cmd);
		i++;
	}
	return (NULL);
}

char	*build_path(char *cmd)
{
	char	*path2cmd;
	char	**paths;
	t_env	*pathenv;

	if (!cmd || ft_strlen(cmd) == 0)
		return (NULL);
	if (ft_strcmp(cmd, ".") == 0 || ft_strcmp(cmd, "..") == 0)
		return (NULL);
	paths = NULL;
	pathenv = env_get(g_env, "PATH");
	if (pathenv && pathenv->value)
		if (!(paths = ft_split(pathenv->value, ':')))
			die("failed to split path");

	if (ft_strchr(cmd, '/') || ft_strslen(paths) == 0)
	{
		path2cmd = build_absolute_path(cmd);
		return (check_path(cmd, path2cmd));
	}
	path2cmd = find_from_env(cmd, paths);
	ft_split_free_null(paths);
	return (path2cmd);
}

/*
** Execute
*/

char	**process_words(char *argv[])
{
	t_wordexp	w;
	size_t		i;

	if (!argv)
		return (NULL);
	w.wordc = 0;
	w.wordv = NULL;
	i = 0;
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

pid_t		start_command(char *argv[], int ispipe, int haspipe, int lastpipe[2])
{
	pid_t	pid;
	int		newpipe[2];
	char	**envp;
	char	*pathname;

	if (ispipe)
		pipe(newpipe);
	pid = fork();
	if (pid < 0)
		die("Failed to fork");
	else if (pid == 0)
	{
		set_signal_handler(SIG_DFL);
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

		argv = process_redir(argv, NULL, NULL);
		if (argv == NULL)
			exit(1);
		argv = process_words(argv);
		if (argv == NULL)
			exit(0);

		if (is_builtin(argv[0], 0))
			exit(exec_builtin(argv));

		if ((pathname = build_path(argv[0])) == NULL)
		{
			ft_putstr_fd("minishell: command not found\n", 2);
			exit(127);
		}
		envp = env_make_envp(g_env, 0);
		if (execve(pathname, argv, envp) < 0)
		{
			perror("failed to execve");
			_exit(1);
		}
		perror("This should not be printed if execvp is successful");
		_exit(1);
	}
	set_signal_handler(SIG_IGN);
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
		ispipe = c->op == OP_PIPE ? 1 : 0;
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

void	wait_pipeine(pid_t pid)
{
	pid_t	exited_pid;
	int		status;

	exited_pid = waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		g_exit_status = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		g_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", 2);
		else
			ft_putstr_fd("\n", 2);
	}
	else
		die("child exited abnormally");
	while (wait(NULL) > 0);
}

void	run_list(t_command *c)
{
	pid_t	exited_pid;
	int		status;

	while (c)
	{
		if (is_builtin(c->argv[0], 0) && c->op != OP_PIPE)
		{
			if ((g_exit_status = exec_builtin_parent(c)) < 0)
				die("exec builtin failed.");
			c = c->next;
			continue ;
		}
		c = do_pipeline(c);
		if (c->pid != -1)
		{
			exited_pid = waitpid(c->pid, &status, 0);
			assert(exited_pid == c->pid);
			if (WIFEXITED(status))
			{
				g_exit_status = WEXITSTATUS(status);
			}
			else if (WIFSIGNALED(status))
			{
				g_exit_status = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGQUIT)
					ft_putstr_fd("Quit: 3\n", 2);
				else
					ft_putstr_fd("\n", 2);
			}
			else
				die("child exited abnormally");
			while (wait(NULL) > 0);
		}
		c = c->next;
	}
}

int			run_one_command(char *commandline)
{
	t_command	*c;

	c = command_new();
	set_signal_handler(handler);
	if (parse(commandline, c) < 0)
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		g_exit_status = 2;
	}
	else if (c->argc)
		run_list(c);
	command_lstclear(&c);
	exit(g_exit_status);
	return (0);
}

void		shell_initialize(void)
{
	g_exit_status = 0;
	g_interrupt = 0;
	g_env = env_init();
	if (!(g_path = ft_split(env_get(g_env, "PATH")->value, ':')))
		die(strerror(errno));
}

int			main(int argc, char *argv[])
{
	char		*commandline;
	t_command	*c;
	int			ret;
	int			needprompt;

	shell_initialize();
	if (argc == 3 && ft_strcmp(argv[1], "-c") == 0)
		return (run_one_command(argv[2]));
	if (argc != 1)
		return (42);
	needprompt = 1;
	commandline = NULL;
	while (42)
	{
		set_signal_handler(handler);
		g_interrupt = 0;
		if (needprompt)
		{
			ft_putstr_fd("minishell>", 2);
			needprompt = 0;
		}
		ret = get_next_commandline(0, &commandline);
		if (ret < 0)
			die("gnc failed.");
		if (ret == 0)
			ft_putstr_fd("\033[0K", 2);
		if (ret == 0 && *commandline == '\0')
		{
			ft_putstr_fd("exit\n", 2);
			exit(1);
		}
		if (ret == 1)
		{
			c = command_new();
			if (parse(commandline, c) < 0)
			{
				ft_putstr_fd("minishell: syntax error\n", 2);
				g_exit_status = 258;
			}
			else if (c->argc)
				run_list(c);
			free(commandline);
			commandline = NULL;
			command_lstclear(&c);
			needprompt = 1;
		}
	}
	return (0);
}
