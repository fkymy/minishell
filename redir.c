/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 18:14:27 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/05 14:22:12 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

static char	*expand_filename(char *arg)
{
	char	*filename;
	char	**word;

	word = wordexp_wrap(arg);
	if (!word || ft_strslen(word) != 1 || ft_strlen(*word) == 0)
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", 2);
		if (word)
			free(*word);
		free(word);
		return (NULL);
	}
	filename = *word;
	free(word);
	return (filename);
}

static int	redirect(int fd, int stdfd, int *save)
{
	if (fd == -1)
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (fd);
	}
	if (save && *save == -1)
		*save = dup(stdfd);
	dup2(fd, stdfd);
	close(fd);
	return (fd);
}

static char	**redirect_free(char **argv, int *in, int *out)
{
	char	*filename;
	int		fd;

	fd = 0;
	if (!(filename = expand_filename(argv[1])))
		return (NULL);
	if (ft_strcmp(*argv, "<") == 0)
		fd = redirect(open(filename, O_RDONLY), 0, in);
	else if (ft_strcmp(*argv, ">") == 0)
		fd = redirect(open(filename,
					O_WRONLY | O_CREAT | O_TRUNC, 0666), 1, out);
	else if (ft_strcmp(*argv, ">>") == 0)
		fd = redirect(open(filename,
					O_WRONLY | O_CREAT | O_APPEND, 0666), 1, out);
	free(filename);
	if (fd == -1)
		return (NULL);
	free(*argv);
	free(argv[1]);
	return (argv + 2);
}

static char	**return_free(char **argv, int i, char **newargv)
{
	while (argv[i])
		free(argv[i++]);
	free(argv);
	i = 0;
	while (newargv[i])
		free(newargv[i++]);
	free(newargv);
	return (NULL);
}

char		**process_redir(char **argv, int *in, int *out)
{
	int		i;
	int		j;
	char	**newargv;

	if (!argv)
		return (NULL);
	if (!(newargv = ft_calloc((ft_strslen(argv) + 1), sizeof(char *))))
		die(strerror(errno));
	i = 0;
	j = 0;
	while (argv[i])
	{
		if (!isredir(argv[i]))
			newargv[j++] = argv[i++];
		else
		{
			if (!(redirect_free(argv + i, in, out)))
				return (return_free(argv, i, newargv));
			i += 2;
		}
	}
	free(argv);
	return (newargv);
}
