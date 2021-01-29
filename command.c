/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 19:37:34 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/24 18:22:12 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "minishell.h"

t_command	*command_new(void)
{
	t_command	*c;

	c = malloc(sizeof(t_command));
	if (!c)
		return (NULL);
	c->next = NULL;
	c->argc = 0;
	c->argv = NULL;
	c->op = 0;
	c->pid = -1;
	return (c);
}

void		command_clear_args(char **argv)
{
	int	i;

	if (argv == NULL)
		return ;
	i = 0;
	while (argv[i])
		free(argv[i++]);
	free(argv);
}

void		command_lstclear(t_command **c)
{
	t_command	*tmp;

	if (c == NULL)
		return ;
	while (*c)
	{
		tmp = (*c)->next;
		command_clear_args((*c)->argv);
		free(*c);
		*c = tmp;
	}
}

int			command_append_arg(t_command *c, char *word)
{
	char	**new_argv;
	int		i;

	new_argv = malloc(sizeof(char *) * (c->argc + 2));
	if (!new_argv)
		return (-1);
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
	return (0);
}
