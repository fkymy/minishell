/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 18:14:27 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/21 17:41:01 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#include "libft/libft.h"
#include "minishell.h"

char	**handle_redir(char **argv)
{
	int i;
	int j;
	int fd;
	int argc;
	char **newargv;

	argc = 0;
	while (argv[argc])
		argc++;
	newargv = malloc(sizeof(char *) * (argc + 1));
	if (!newargv)
		die(strerror(errno));

	i = 0;
	j = 0;
	fd = -1;
	while (i < argc)
	{
		if (ft_strcmp(argv[i], "<") == 0)
		{
			fd = open(argv[i + 1], O_RDONLY);
			if (fd == -1)
				die(strerror(errno));
			dup2(fd, 0);
			close(fd);
			free(argv[i]);
			free(argv[i + 1]);
			i += 2;
		}
		else if (ft_strcmp(argv[i], ">") == 0)
		{
			fd = open(argv[i + 1], O_WRONLY|O_CREAT|O_TRUNC, 0666);
			if (fd == -1)
				die(strerror(errno));
			dup2(fd, 1);
			close(fd);
			free(argv[i]);
			free(argv[i + 1]);
			i += 2;
		}
		else if (ft_strcmp(argv[i], ">>") == 0)
		{
			fd = open(argv[i + 1], O_WRONLY|O_CREAT|O_APPEND, 0666);
			if (fd == -1)
				die(strerror(errno));
			dup2(fd, 1);
			close(fd);
			free(argv[i]);
			free(argv[i + 1]);
			i += 2;
		}
		else
			newargv[j++] = argv[i++];
	}
	newargv[j] = NULL;
	free(argv);
	return (newargv);
}
