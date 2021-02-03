/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 18:14:27 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/03 13:10:28 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#include "libft/libft.h"
#include "minishell.h"

static char	*expand_filename(char *arg)
{
	char	*filename;
	char	**word;

	word = wordexp_wrap(arg);
	if (!word || ft_strslen(word) != 1)
		die("minishell: ambiguous redirect (word null)");
	if (ft_strslen(word) != 1)
		die("minishell: ambiguous redirect (more than one word)");
	if (ft_strlen(*word) == 0)
		die("minishell: ambiguous redirect (word is empty)");
	filename = *word;
	free(word);
	return (filename);
}

static void	redirect(int fd, int stdfd, int *save)
{
	if (fd == -1)
		die(strerror(errno));
	if (save)
		*save = dup(stdfd);
	dup2(fd, stdfd);
	close(fd);
}

static char	**redirect_free(char **argv, int *in, int *out)
{
	char	*filename;

	if (!(filename = expand_filename(argv[1])))
		die(strerror(errno));
	if (ft_strcmp(*argv, "<") == 0)
		redirect(open(filename, O_RDONLY), 0, in);
	else if (ft_strcmp(*argv, ">") == 0)
		redirect(open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666), 1, out);
	else if (ft_strcmp(*argv, ">>") == 0)
		redirect(open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666), 1, out);
	free(filename);
	free(*argv);
	free(argv[1]);
	return (argv + 2);
}

char	**process_redir(char **argv, int *in, int *out)
{
	int		i;
	int		j;
	char	**newargv;

	if (!argv)
		return (NULL);
	if (!(newargv = malloc(sizeof(char *) * (ft_strslen(argv) + 1))))
		die(strerror(errno));
	i = 0;
	j = 0;
	while (argv[i])
	{
		if (!isredir(argv[i]))
			newargv[j++] = argv[i++];
		else
		{
			redirect_free(argv + i, in, out);
			i += 2;
		}
	}
	newargv[j] = NULL;
	free(argv);
	return (newargv);
}
