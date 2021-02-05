/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordexp_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 17:07:12 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/05 17:48:03 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

char	*expand_status(char *str, t_vector *v)
{
	char	*status;

	if (!(status = ft_itoa(g_exit_status)))
		die("malloc failed");
	vector_appends(v, status);
	free(status);
	return (str + 2);
}

char	*expand_var(char *str, t_vector *v)
{
	char	*name;
	t_env	*e;
	int		i;

	++str;
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	name = ft_substr(str, 0, i);
	e = env_get(g_env, name);
	if (e && e->value != NULL)
		vector_appends(v, e->value);
	free(name);
	return (str + i);
}

char	*expand(char *str, t_vector *v)
{
	if (*str != '$')
	{
		return (str);
	}
	if (str[1] == '?')
	{
		return (expand_status(str, v));
	}
	if (!ft_isalpha(str[1]) && str[1] != '_')
	{
		vector_append(v, *str);
		return (str + 1);
	}
	return (expand_var(str, v));
}

char	*shift_expansion(char *word, t_wordexp *w)
{
	t_vector	v;
	size_t		i;
	char		**fields;

	vector_initialize(&v);
	if (word[1] == '\"')
		return (shift_quotes(word + 1, w));
	word = expand(word, &v);
	if (v.data == NULL)
		return (word);
	fields = ft_split(vector_gets(&v), ' ');
	if (!ft_isspaces(vector_gets(&v))
			&& (w->offset == 0 || (*vector_gets(&v) == ' ')))
		wordexp_append_arg(w, fields[0] ? fields[0] : ft_strdup(""));
	else
		wordexp_join_arg(w, fields[0] ? fields[0] : ft_strdup(""));
	i = 1;
	while (i < ft_strslen(fields))
		wordexp_append_arg(w, fields[i++]);
	free(fields);
	if (ft_last_chr(vector_gets(&v)) == ' ')
		w->offset = 0;
	free(v.data);
	return (word);
}
