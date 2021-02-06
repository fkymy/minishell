/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 17:40:00 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/05 21:25:58 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

char		**process_words(char *argv[])
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

static void	execute_command(char *argv[])
{
	char	**envp;
	char	*pathname;

	if (!(argv = process_redir(argv, NULL, NULL)))
		exit(1);
	if (!(argv = process_words(argv)))
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
		ft_putstr_fd(strerror(errno), 2);
		exit(1);
	}
	ft_putstr_fd(strerror(errno), 2);
	exit(1);
}

pid_t		start_command(char *argv[],
		int ispipe, int haspipe, int lastpipe[2])
{
	pid_t	pid;
	int		newpipe[2];

	if (ispipe)
		pipe(newpipe);
	if ((pid = fork()) < 0)
		die(strerror(errno));
	else if (pid == 0)
	{
		set_signal_handler(SIG_DFL);
		setup_pipe(ispipe, haspipe, newpipe, lastpipe);
		execute_command(argv);
	}
	set_signal_handler(SIG_IGN);
	cleanup_pipe(haspipe, lastpipe);
	if (ispipe)
	{
		lastpipe[0] = newpipe[0];
		lastpipe[1] = newpipe[1];
	}
	return (pid);
}

void		run_list(t_command *c)
{
	int		condition;

	condition = 1;
	while (c)
	{
		if (condition)
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
				wait_pipeine(c->pid);
		}
		c = c->next;
	}
}
