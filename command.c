/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 19:37:34 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/19 18:35:53 by yufukuya         ###   ########.fr       */
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
	return (c);
}

void		command_lstclear(t_command **c)
{
	t_command	*head;
	t_command	*tmp;

	if (c == NULL)
		return ;
	head = *c;
	while (*c)
	{
		tmp = (*c)->next;
		command_clear(*c);
		*c = tmp;
	}
	head = NULL;
}

void		command_clear(t_command *c)
{
	int	i;

	if (c == NULL)
		return ;
	i = 0;
	while (i < c->argc)
		free(c->argv[i++]);
	free(c);
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
