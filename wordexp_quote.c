/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordexp_quote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 17:02:36 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/02 17:06:05 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

static char	*unquote_single(char *str, t_vector *v)
{
	if (*str != '\'')
		return (str);
	++str;
	while (*str != '\'')
	{
		vector_append(v, *str);
		++str;
	}
	return (str);
}

static int	is_double_quote_escapable(int c)
{
	return (c == '$' || c == '\\' || c == '\"');
}

static char	*unquote_double(char *str, t_vector *v)
{
	int	escaped;

	if (*str != '\"')
		return (str);
	++str;
	while (*str != '\"')
	{
		escaped = 0;
		if (*str == '\\'
				&& is_double_quote_escapable(str[1]))
		{
			escaped = 1;
			++str;
		}
		if (*str == '$' && !escaped)
		{
			str = expand(str, v);
		}
		else
		{
			vector_append(v, *str);
			++str;
		}
	}
	return (str);
}

char	*shift_quotes(char *word, t_wordexp *w)
{
	t_vector	v;

	vector_initialize(&v);
	while (*word == '\'' || *word == '\"')
	{
		if (*word == '\'')
			word = unquote_single(word, &v);
		else if (*word == '\"')
			word = unquote_double(word, &v);
		word++;
	}
	if (w->offset)
		wordexp_join_arg(w, vector_gets(&v));
	else
		wordexp_append_arg(w, vector_gets(&v));
	return (word);
}
