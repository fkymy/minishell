/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 18:44:13 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/11 21:29:54 by yufukuya         ###   ########.fr       */
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

#include "libft/libft.h"
#include "command.h"
#include "vector_string.h"

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


/* Logger */

/* void Log(char *fmt, ...) */
/* { */
/*  */
/* } */



/* Utilities */

int		ft_isspace(int c)
{
	return (c == '\t' || c == '\n' || c == '\v' ||
			c == '\f' || c == '\r' || c == ' ');
}

int		ft_isspecial(int c)
{
	return (c == '<' || c == '>' || c == '&' || c == '|' ||
			c == ';' || c == '(' || c == ')' || c == '#');
}

int		isredirect(int c)
{
	return (c == '<' || c == '>');
}

int		isandor(char *s)
{
	return ((*s == '&' || *s == '|') && s[1] == *s);
}

int		token_isop(int t)
{
	return (t == TOKEN_SEPARATOR || t == TOKEN_PIPE
			|| t == TOKEN_AND || t == TOKEN_OR);
}

int		token_ispipe(int t)
{
	return (t == TOKEN_PIPE);
}

int		token_to_op(int t)
{
	if (t == TOKEN_SEPARATOR)
		return (OP_SEPARATOR);
	if (t == TOKEN_PIPE)
		return (OP_PIPE);
	return (0);
}

char	*get_next_token(char *str, int *type, char **token)
{
	t_vector_string v;

	vector_initialize(&v);

	while (str && ft_isspace(*str))
		++str;
	if (!str || *str == '\0' || *str == '#')
	{
		*type = TOKEN_SEPARATOR;
		*token = NULL;
		return (NULL);
	}

	if (isredirect(*str))
	{
		*type = TOKEN_REDIRECTION;
		vector_append(&v, *str);
		if (str[1] == '>')
		{
			vector_append(&v, str[1]);
			++str;
		}
		++str;
	}
	else if (isandor(str))
	{
		*type = *str == '&' ? TOKEN_AND : TOKEN_OR;
		vector_append(&v, *str);
		vector_append(&v, str[1]);
		str += 2;
	}
	else if (ft_isspecial(*str))
	{
		if (*str == ';' || *str == '&')
			*type = TOKEN_SEPARATOR; // Do not support background conditionals
		else if (*str == '|')
			*type = TOKEN_PIPE;
		else
			*type = TOKEN_OTHER;
		vector_append(&v, *str);
		++str;
	}
	else
	{
		*type = TOKEN_WORD;
		int quoted = 0;
		while ((*str && quoted)
				|| (*str && !ft_isspace(*str)
					&& !ft_isspecial(*str)))
		{
			if ((*str == '\"' || *str == '\'') && !quoted)
				quoted = *str;
			else if (*str == quoted)
				quoted = 0;
			else if (*str == '\\' && str[1] != '\0' && quoted != '\'')
			{
				vector_append(&v, str[1]);
				++str;
			}
			else
				vector_append(&v, *str);
			++str;
		}
	}

	vector_append(&v, '\0');
	*token = v.data;
	return (str);
}


/* Execute */

// A list is composed of pipelines joined by ; or &
// A pipeline is composed of commands joined by |
// A command is composed of words and redirections

int haspipe = 0;
int lastpipe[2] = { -1, -1 };
int currpipe[2];

pid_t	start_command(t_command *c)
{
	pid_t pid;

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
		if (haspipe)
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
	if (haspipe)
	{
		close(lastpipe[0]);
		close(lastpipe[1]);
	}

	// Setup for next command
	haspipe = token_ispipe(c->op);
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
	int stat_loc;
	pid_t exited_pid;

	while (c)
	{
		/* c = run_pipeline(c); */
		/* exited_pid = waitpid(pid, &stat_loc, 0); */
		/* if (WIFEXITED(stat_loc)) */
		/* 	printf("%d\n", WEXITSTATUS(stat_loc)); */
		/* else */
		/* 	printf("child exited abnormally\n"); */
		/* c = c->next; */

		// Validations
		if (c->op == -1 || !c->argv || c->argv[c->argc] != NULL)
		{
			printf("Error: invalid command in list.\n");
			c = c->next;
			continue ;
		}

		// Run pipeline in parallel
		pid_t pid;
		while (c)
		{
			pid = start_command(c);
			if (haspipe)
				c = c->next;
			else
				break ;
		}

		// Wait for child (blocking)
		exited_pid = waitpid(pid, &stat_loc, 0);
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

int		main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: ./minishell 'commandline here'\n");
		exit(1);
	}

	// Parse
	t_command *c;
	c = parse_commandline(argv[1]);

	// Execute
	if (c->argc)
		run_list(c);

	// Cleanup
	// - reap zombie processes
	while(waitpid(-1,NULL,WNOHANG) > 0);

	// - free
	t_command *tmp;
	while (c)
	{
		tmp = c->next;
		command_clear(c);
		c = tmp;
	}

	exit(0);
}

