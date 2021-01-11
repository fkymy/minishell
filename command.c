/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 19:37:34 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/11 19:45:59 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * command is a linked list parsed from a commandline string.
 * op is the connecting opeartor ; and | (redirection is not op)
 */

#include <stdlib.h>

#include "command.h"

t_command	*command_new(void)
{
	t_command	*c;

	if (!(c = malloc(sizeof(t_command))))
		return (NULL);
	c->next = NULL;
	c->argc = 0;
	c->argv = NULL;
	c->op = 0;

	c->pid = -1;
	return (c);
}

void		command_clear(t_command *c)
{
	int	i;

	i = 0;
	while (i < c->argc)
		free(c->argv[i++]);
	free(c);
}

void		command_append_arg(t_command *c, char *word)
{
	char	**new_argv;
	int		i;

	new_argv = malloc(sizeof(char *) * (c->argc + 2));
	i = 0;
	while (i < c->argc)
	{
		new_argv[i] = c->argv[i];
		i++;
	}
	if (c->argv)
		free(c->argv);
	c->argv = new_argv;
	c->argv[c->argc] = word;
	c->argv[c->argc + 1] = NULL;
	++c->argc;
}