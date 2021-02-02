/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordexp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 11:42:13 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/02 17:07:56 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft/libft.h"
#include "minishell.h"

void		wordexp_join_arg(t_wordexp *w, char *s)
{
	char	*new_arg;

	new_arg = ft_strjoin(w->wordv[w->wordc - 1], s);
	free(w->wordv[w->wordc - 1]);
	w->wordv[w->wordc - 1] = new_arg;
	w->offset = ft_strlen(new_arg);
	free(s);
}

int		wordexp_append_arg(t_wordexp *w, char *word)
{
	char	**new_argv;
	size_t	i;

	new_argv = malloc(sizeof(char *) * (w->wordc + 2));
	if (!new_argv)
		return (-1);
	i = 0;
	while (i < w->wordc)
	{
		new_argv[i] = w->wordv[i];
		i++;
	}
	if (w->wordv)
		free(w->wordv);
	w->wordv = new_argv;
	w->wordv[w->wordc] = word;
	w->wordv[w->wordc + 1] = NULL;
	++w->wordc;
	w->offset = ft_strlen(word);
	return (0);
}

char	*shift_word(char *word, t_wordexp *w)
{
	t_vector	v;

	vector_initialize(&v);
	while (*word && *word != '\'' && *word != '\"' && *word != '$')
	{
		vector_append(&v, *word);
		++word;
	}
	if (w->offset)
		wordexp_join_arg(w, vector_gets(&v));
	else
		wordexp_append_arg(w, vector_gets(&v));
	return (word);
}

int		wordexp(char *word, t_wordexp *w)
{
	if (!word)
		return (-1);
	while (*word)
	{
		if ((*word == '\'' || *word == '\"'))
			word = shift_quotes(word, w);
		else if (*word == '$')
			word = shift_expansion(word, w);
		else
			word = shift_word(word, w);
	}
	return (0);
}

char	**wordexp_wrap(char *word)
{
	t_wordexp	w;

	w.wordc = 0;
	w.wordv = NULL;
	w.offset = 0;

	if (wordexp(word, &w) < 0)
		die("wordexp failed");
	return (w.wordv);
}
