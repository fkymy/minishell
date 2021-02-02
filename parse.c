/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 17:47:51 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/31 20:15:41 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

#include "libft/libft.h"
#include "minishell.h"

int			isredir(char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, ">") == 0)
		return (1);
	if (ft_strcmp(str, "<") == 0)
		return (1);
	if (ft_strcmp(str, ">>") == 0)
		return (1);
	return (0);
}

int			set_operator(t_command *c, int op)
{
	if (c->op || !c->argc)
		return (-1);
	if (isredir(c->argv[c->argc - 1]))
		return (-1);
	c->op = op;
	return (0);
}

int			set_word(t_command **c, int type, char *word)
{
	t_command	*new;

	if ((*c)->op)
	{
		if (!(new = command_new()))
			die("parse failed");
		(*c)->next = new;
		*c = new;
	}
	if ((*c)->argc && isredir((*c)->argv[(*c)->argc - 1])
			&& type == TOKEN_REDIRECTION)
	{
		return (-1);
	}
	if (command_append_arg(*c, word) < 0)
		die("parse failed");
	return (0);
}

// check syntax error
//
// - only control operator ( ; )
// - operator comes first ( ; echo )
// - double control operator ( echo hello ; ; )
// - operator after redirection
//
// - double redirection operator
// - echo hello > ; exit this should not pass through.
int			parse(char *commandline, t_command **c)
{
	int			type;
	char		*token;
	t_command	*current;

	if (!(*c = command_new()))
		die("parse failed");
	current = *c;
	while ((commandline = get_next_token(commandline, &type, &token)) != NULL) {
		if (token_isop(type))
		{
			free(token);
			if (set_operator(current, type) < 0)
				return (-1);
		}
		else
		{
			if (set_word(&current, type, token) < 0)
			{
				free(token);
				return (-1);
			}
		}
	}
	if (type == TOKEN_OTHER)
		die("token failed");
	if (current->argc && !current->op)
		return (set_operator(current, TOKEN_SEPARATOR));
	// check list
	/* if (c->op == -1 || !c->argv || c->argv[c->argc] != NULL) */
	/* { */
	/* 	ft_putstr_fd("Error: invalid command in list.\n", 2); */
	/* 	c = c->next; */
	/* 	continue ; */
	/* } */
	return (0);
}
