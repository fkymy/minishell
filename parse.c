/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 17:47:51 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/05 11:23:47 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

#include "libft/libft.h"
#include "minishell.h"

static int			iscontrolop(int t)
{
	return (t == TOKEN_SEPARATOR || t == TOKEN_PIPE
			|| t == TOKEN_AND || t == TOKEN_OR);
}

static t_command	*set_operator(t_command *c, int op)
{
	if (!c)
		return (NULL);
	if (c->op || !c->argc)
		return (NULL);
	if (isredir(c->argv[c->argc - 1]))
		return (NULL);
	c->op = op;
	return (c);
}

static t_command	*set_word(t_command *c, int type, char *word)
{
	t_command	*new;

	if (!c)
		return (NULL);
	if (c->op)
	{
		if (!(new = command_new()))
			die("parse failed");
		c->next = new;
		c = new;
	}
	if (c->argc && isredir(c->argv[c->argc - 1])
			&& type == TOKEN_REDIRECTION)
	{
		return (NULL);
	}
	if (command_append_arg(c, word) < 0)
		die("parse failed");
	return (c);
}

int					parse(char *commandline, t_command *c)
{
	int		type;
	char	*token;

	while ((commandline = get_next_token(commandline, &type, &token)) != NULL)
	{
		if (iscontrolop(type))
		{
			free(token);
			if (!set_operator(c, type))
				return (-1);
		}
		else
		{
			if (!(c = set_word(c, type, token)))
			{
				free(token);
				return (-1);
			}
		}
	}
	if (c->argc && c->op != TOKEN_SEPARATOR)
		if (!set_operator(c, TOKEN_SEPARATOR))
			return (-1);
	return (0);
}
