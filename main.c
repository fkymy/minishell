/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 18:44:13 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/06 18:01:50 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

#include "parse.h"

int		execute(char **args)
{
	pid_t p;

	p = fork();
	if (p < 0)
	{
		perror("Failed to fork");
		exit(1);
	}

	if (p == 0)
	{
		if (execvp(args[0], args) < 0)
		{
			perror("Failed to execve");
			exit(1);
		}
		printf("This should not print if execvp is successful\n");
		exit(1);
	}

	int status;
	pid_t exited_pid = waitpid(p, &status, 0); // block
	assert(exited_pid == p);
	if (WIFEXITED(status))
	{
		printf("Child with pid %d exited with status %d\n", exited_pid, WEXITSTATUS(status));
	}
	else
	{
		printf("Child exited abnormally\n");
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	if (argc > 1 && ft_strncmp("-c", argv[1], 2) == 0)
	{
		char **args;
		if (!(args = ft_split(argv[2], ' ')))
		{
			perror("Failed to parse");
			exit(1);
		}
		if (args[0] == NULL)
		{
			perror("No args");
			exit(1);
		}
		execute(args);
		exit(1);
	}
	else
	{
		/* Interactive loop */
		while (1)
		{
			write(1, "minishell> ", 11);
			// 1. Read
			// 2. Parse
			// 3. Execute
		}

	}

	return (0);
}
