/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 17:36:33 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/05 17:52:05 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>

#include "libft/libft.h"
#include "minishell.h"

void		setup_pipe(int ispipe, int haspipe, int newpipe[2], int lastpipe[2])
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
}

void		cleanup_pipe(int haspipe, int lastpipe[2])
{
	if (haspipe)
	{
		close(lastpipe[0]);
		close(lastpipe[1]);
	}
}

t_command	*do_pipeline(t_command *c)
{
	int	ispipe;
	int	haspipe;
	int	lastpipe[2];

	ispipe = 0;
	haspipe = 0;
	lastpipe[0] = -1;
	lastpipe[1] = -1;
	while (c)
	{
		ispipe = c->op == OP_PIPE ? 1 : 0;
		c->pid = start_command(c->argv, ispipe, haspipe, lastpipe);
		haspipe = ispipe;
		if (ispipe && c->next)
			c = c->next;
		else if (ispipe && !c->next)
			die("failed to do pipelin");
		else
			break ;
	}
	return (c);
}

void		wait_pipeine(pid_t pid, int *condition)
{
	pid_t	exited_pid;
	int		status;

	(void)condition;
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
	while (wait(NULL) > 0)
		;
}
