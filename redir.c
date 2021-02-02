/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 18:14:27 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/02 14:45:59 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#include "libft/libft.h"
#include "minishell.h"

char	**handle_redir(char **argv, int *in, int *out)
{
	int		i;
	int		j;
	int		fd;
	int		argc;
	char	**newargv;

	if (!argv)
		return (NULL);
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
		if (!isredir(argv[i]))
		{
			newargv[j++] = argv[i++];
			continue ;
		}

		char **word = wordexp_wrap(argv[i + 1]);
		if (ft_strslen(word) != 1)
			die("redir operand should not expand to more than one word.");

		if (ft_strcmp(argv[i], "<") == 0)
		{
			if (in != NULL)
			{
				*in = dup(0);
			}
			fd = open(*word, O_RDONLY);
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
			if (out != NULL)
			{
				*out = dup(1);
			}
			fd = open(*word, O_WRONLY|O_CREAT|O_TRUNC, 0666);
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
			if (out != NULL)
			{
				*out = dup(1);
			}
			fd = open(*word, O_WRONLY|O_CREAT|O_APPEND, 0666);
			if (fd == -1)
				die(strerror(errno));
			dup2(fd, 1);
			close(fd);
			free(argv[i]);
			free(argv[i + 1]);
			i += 2;
		}
		free(*word);
		free(word);
	}
	newargv[j] = NULL;
	free(argv);
	return (newargv);
}
