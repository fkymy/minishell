/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 18:44:13 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/06 14:45:11 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int			g_exit_status;
t_env		*g_env;
char		*g_pwd;

void		die(char *msg)
{
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	exit(1);
}

static void	shell_initialize(void)
{
	g_exit_status = 0;
	g_interrupt = 0;
	g_env = env_init();
}

static void	shell_reinitialize(void)
{
	set_signal_handler(handler);
	g_interrupt = 0;
}

static void	run_commandline(int ret, char **commandline)
{
	t_command	*c;

	if (ret == 0)
		ft_putstr_fd("\033[0K", 2);
	if (ret == 0 && **commandline == '\0')
	{
		ft_putstr_fd("exit\n", 2);
		exit(1);
	}
	if (ret == 1)
	{
		c = command_new();
		if (parse(*commandline, c) < 0)
		{
			ft_putstr_fd("minishell: syntax error\n", 2);
			g_exit_status = 258;
		}
		else if (c->argc)
			run_list(c);
		free(*commandline);
		*commandline = NULL;
		command_lstclear(&c);
	}
}

int			main(int argc, char *argv[])
{
	char	*commandline;
	int		ret;
	int		needprompt;

	(void)argv;
	shell_initialize();
	if (argc != 1)
		return (42);
	needprompt = 1;
	commandline = NULL;
	while (42)
	{
		shell_reinitialize();
		if (needprompt)
		{
			ft_putstr_fd("minishell>", 2);
			needprompt = 0;
		}
		if ((ret = get_next_commandline(0, &commandline)) < 0)
			die("gnc failed.");
		run_commandline(ret, &commandline);
		needprompt = ret == 1 ? 1 : 0;
	}
	return (0);
}
