/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 21:53:11 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/11 21:55:59 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "vector_string.h"
#include "token.h"

static int	ft_isspace(int c)
{
	return (c == '\t' || c == '\n' || c == '\v' ||
			c == '\f' || c == '\r' || c == ' ');
}

static int	ft_isspecial(int c)
{
	return (c == '<' || c == '>' || c == '&' || c == '|' ||
			c == ';' || c == '(' || c == ')' || c == '#');
}

static int	isredirect(int c)
{
	return (c == '<' || c == '>');
}

static int	isandor(char *s)
{
	return ((*s == '&' || *s == '|') && s[1] == *s);
}

int			token_isop(int t)
{
	return (t == TOKEN_SEPARATOR || t == TOKEN_PIPE
			|| t == TOKEN_AND || t == TOKEN_OR);
}

int			token_ispipe(int t)
{
	return (t == TOKEN_PIPE);
}

char		*get_next_token(char *str, int *type, char **token)
{
	t_vector_string v;

	vector_initialize(&v);

	while (str && ft_isspace(*str))
		++str;
	if (!str || *str == '\0' || *str == '#')
	{
		*type = TOKEN_SEPARATOR;
		*token = NULL;
		return (NULL);
	}

	if (isredirect(*str))
	{
		*type = TOKEN_REDIRECTION;
		vector_append(&v, *str);
		if (str[1] == '>')
		{
			vector_append(&v, str[1]);
			++str;
		}
		++str;
	}
	else if (isandor(str))
	{
		*type = *str == '&' ? TOKEN_AND : TOKEN_OR;
		vector_append(&v, *str);
		vector_append(&v, str[1]);
		str += 2;
	}
	else if (ft_isspecial(*str))
	{
		if (*str == ';' || *str == '&')
			*type = TOKEN_SEPARATOR; // Do not support background conditionals
		else if (*str == '|')
			*type = TOKEN_PIPE;
		else
			*type = TOKEN_OTHER;
		vector_append(&v, *str);
		++str;
	}
	else
	{
		*type = TOKEN_WORD;
		int quoted = 0;
		while ((*str && quoted)
				|| (*str && !ft_isspace(*str)
					&& !ft_isspecial(*str)))
		{
			if ((*str == '\"' || *str == '\'') && !quoted)
				quoted = *str;
			else if (*str == quoted)
				quoted = 0;
			else if (*str == '\\' && str[1] != '\0' && quoted != '\'')
			{
				vector_append(&v, str[1]);
				++str;
			}
			else
				vector_append(&v, *str);
			++str;
		}
	}

	vector_append(&v, '\0');
	*token = v.data;
	return (str);
}