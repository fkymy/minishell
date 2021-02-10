/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 21:53:11 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/10 17:26:55 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

char		*tokenize_redir(t_vector *v, char *str, int *type)
{
	*type = TOKEN_REDIRECTION;
	vector_append(v, *str);
	if (str[1] == '>')
	{
		vector_append(v, str[1]);
		++str;
	}
	++str;
	return (str);
}

char		*tokenize_andor(t_vector *v, char *str, int *type)
{
	*type = *str == '&' ? TOKEN_AND : TOKEN_OR;
	vector_append(v, *str);
	vector_append(v, str[1]);
	str += 2;
	return (str);
}

char		*tokenize_shellspecial(t_vector *v, char *str, int *type)
{
	if (*str == ';' || *str == '&')
		*type = TOKEN_SEPARATOR;
	else if (*str == '|')
		*type = TOKEN_PIPE;
	else
		*type = TOKEN_OTHER;
	vector_append(v, *str);
	++str;
	return (str);
}

char		*buildtoken(t_vector *v, char *str, int *type)
{
	int	quoted;

	if (*str == '<' || *str == '>')
		return (tokenize_redir(v, str, type));
	if ((*str == '&' || *str == '|') && *str == str[1])
		return (tokenize_andor(v, str, type));
	if (isshellspecial(*str))
		return (tokenize_shellspecial(v, str, type));
	*type = TOKEN_WORD;
	quoted = 0;
	while ((*str && quoted)
			|| (*str && !ft_isspace(*str) && !isshellspecial(*str)))
	{
		if ((*str == '\"' || *str == '\'') && !quoted)
			quoted = *str;
		else if (*str == quoted)
			quoted = 0;
		else if (*str == '\\' && quoted == '\"'
				&& (str[1] == '\"' || str[1] == '\\'))
			vector_append(v, *str++);
		vector_append(v, *str++);
	}
	v->error = quoted ? 1 : 0;
	return (str);
}

char		*get_next_token(char *str, int *type, char **token)
{
	t_vector v;

	vector_initialize(&v);
	while (str && ft_isspace(*str))
		++str;
	if (!str || *str == '\0')
	{
		*type = TOKEN_SEPARATOR;
		*token = NULL;
		return (NULL);
	}
	str = buildtoken(&v, str, type);
	if (v.error)
		die("you cannot do mutiline commands.");
	*token = vector_gets(&v);
	return (str);
}
