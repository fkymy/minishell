/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 18:14:27 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/19 18:40:20 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#include "libft/libft.h"
#include "minishell.h"

void		handle_redir(t_command *c)
{
	int i;
	int j;
	int fd;
	char **newargv;

	newargv = malloc(sizeof(char *) * (c->argc + 1));
	if (!newargv)
		die(strerror(errno));

	i = 0;
	j = 0;
	fd = -1;
	while (i < c->argc)
	{
		if (ft_strcmp(c->argv[i], "<") == 0)
		{
			fd = open(c->argv[i + 1], O_RDONLY);
			if (fd == -1)
				die(strerror(errno));
			dup2(fd, 0);
			close(fd);
			free(c->argv[i]);
			free(c->argv[i + 1]);
			i += 2;
		}
		else if (ft_strcmp(c->argv[i], ">") == 0)
		{
			fd = open(c->argv[i + 1], O_WRONLY|O_CREAT|O_TRUNC, 0666);
			if (fd == -1)
				die(strerror(errno));
			dup2(fd, 1);
			close(fd);
			free(c->argv[i]);
			free(c->argv[i + 1]);
			i += 2;
		}
		else if (ft_strcmp(c->argv[i], ">>") == 0)
		{
			fd = open(c->argv[i + 1], O_WRONLY|O_CREAT|O_APPEND, 0666);
			if (fd == -1)
				die(strerror(errno));
			dup2(fd, 1);
			close(fd);
			free(c->argv[i]);
			free(c->argv[i + 1]);
			i += 2;
		}
		else
			newargv[j++] = c->argv[i++];
	}
	newargv[j] = NULL;
	free(c->argv);
	c->argv = newargv;
}
