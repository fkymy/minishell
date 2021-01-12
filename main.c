/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 18:44:13 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/12 16:52:51 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>

#include "libft/libft.h"
#include "vector_string.h"
#include "token.h"
#include "command.h"

# define TOKEN_WORD 0
# define TOKEN_REDIRECTION 1
# define TOKEN_SEPARATOR 2
# define TOKEN_BACKGROUND 3
# define TOKEN_PIPE 4
# define TOKEN_AND 5
# define TOKEN_OR 6
# define TOKEN_OTHER -1

// is it easier if i separate tokens and command ops?
# define OP_SEPARATOR 1
# define OP_PIPE 2

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

/* Logger */

/* void Log(char *fmt, ...) */
/* { */
/*  */
/* } */


/* Execute */

// A list is composed of pipelines joined by ; or &
// A pipeline is composed of commands joined by |
// A command is composed of words and redirections

pid_t	start_command(t_command *c, int *haspipe, int lastpipe[2])
{
	pid_t	pid;
	int		currpipe[2];

	if (token_ispipe(c->op))
	{
		int r = pipe(currpipe);
		assert(r == 0);
	}

	pid = fork();
	if (pid < 0)
	{
		perror("Failed to fork");
		exit(1);
	}
	else if (pid == 0)
	{
		// Handle pipe
		if (*haspipe)
		{
			close(lastpipe[1]);
			dup2(lastpipe[0], 0);
			close(lastpipe[0]);
		}
		if (token_ispipe(c->op))
		{
			close(currpipe[0]);
			dup2(currpipe[1], 1);
			close(currpipe[1]);
		}

		// Handle redirections
		int fd;
		for (int i = 0; i < c->argc; i++)
		{
			if (ft_strncmp(c->argv[i], "<", 1) == 0) // this would catch <abc
			{
				fd = open(c->argv[i+1], O_RDONLY);
				if (fd == -1)
				{
					perror("Failed to open redirection file");
					_exit(errno);
				}
				dup2(fd, 0);
				close(fd);
				c->argv[i] = NULL;
			}
			else if (ft_strncmp(c->argv[i], ">", 1) == 0)
			{
				fd = open(c->argv[i+1], O_WRONLY|O_CREAT, 0666);
				if (fd == -1)
				{
					perror("Failed to open redirection file");
					_exit(errno);
				}
				dup2(fd, 1);
				close(fd);
				c->argv[i] = NULL;
			}
			else if (ft_strncmp(c->argv[i], ">>", 2) == 0)
			{
				printf("Not implemented yet\n");
			}
		}

		if (execvp(c->argv[0], c->argv) < 0)
		{
			perror("Failed to execvp");
			_exit(1);
		}
		perror("This should not be printed if execvp is successful");
		_exit(1);
	}

	// Cleanup
	if (*haspipe)
	{
		close(lastpipe[0]);
		close(lastpipe[1]);
	}

	// Setup for next command
	*haspipe = token_ispipe(c->op);
	if (token_ispipe(c->op))
	{
		lastpipe[0] = currpipe[0];
		lastpipe[1] = currpipe[1];
	}
	c->pid = pid;
	return (c->pid);
}

// From command list:
//   - Run pipelines (joined by ;)
//     - Run commands in parallel (joined by |)
//       - Redirect if necessary
//     - Handle pipe
//   - Wait for exit and status

void	run_list(t_command *c)
{
	int		stat_loc;
	pid_t	exited_pid;

	while (c)
	{
		// Validations ?
		if (c->op == -1 || !c->argv || c->argv[c->argc] != NULL)
		{
			printf("Error: invalid command in list.\n");
			c = c->next;
			continue ;
		}

		// Run pipeline in parallel
		pid_t command_pid;
		int haspipe = 0;
		int lastpipe[2] = { -1, -1 };

		while (c)
		{
			command_pid = start_command(c, &haspipe, lastpipe);
			if (haspipe)
				c = c->next;
			else
				break ;
		}

		// Wait for child (blocking)
		exited_pid = waitpid(command_pid, &stat_loc, 0);
		assert(exited_pid == c->pid);
		if (WIFEXITED(stat_loc))
		{
			printf("[run_list %d] child with pid %d exited with status %d\n",
					getpid(), exited_pid, WEXITSTATUS(stat_loc));
		}
		else
		{
			printf("[run_list %d] child exited abnormally with status: %d\n",
					getpid(), stat_loc);
		}
		c = c->next;
	}
}


/* Parse */
/*   parse commandline to linked list of commands */

t_command	*parse_commandline(char *str)
{
	int			type;
	char		*token;
	t_command	*head;
	t_command	*c;

	c = command_new();
	head = c;
	while ((str = get_next_token(str, &type, &token)) != NULL) {
		if (token_isop(type))
		{
			if (c->op)
			{
				printf("Error: syntax error near unexpected token %s\n", token);
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

	// Add last ; if necessary
	if (!c->op)
		c->op = type;

	return (head);
}


/* Main */

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

