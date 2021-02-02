/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordexp_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 17:07:12 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/02 17:09:05 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

char	*build_envkey_shift(char **p)
{
	char	*envkey;
	int		len;
	char	*s;

	len = 0;
	s = *p;
	while (s[len] && (ft_isalnum(s[len]) || s[len] == '_'))
		len++;
	if (!(envkey = malloc(sizeof(char) * (len + 2))))
		return (NULL);
	ft_memcpy(envkey, s, len);
	envkey[len] = '=';
	envkey[len + 1] = '\0';
	*p += len;
	return (envkey);
}

char	*expand(char *str, t_vector *v)
{
	char	**envp = env_make_envp(g_env, 0);
	char	*envkey;
	int		i;

	if (*str != '$')
		return (str);
	if (str[1] == '?')
	{
		vector_appends(v, ft_itoa(g_exit_status));
		return (str + 2);
	}
	if (!ft_isalpha(str[1]) && str[1] != '_')
	{
		vector_append(v, *str);
		return (str + 1);
	}
	++str;
	envkey = build_envkey_shift(&str);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], envkey, ft_strlen(envkey)))
		i++;
	if (envp[i])
		vector_appends(v, envp[i] + ft_strlen(envkey));
	free(envkey);
	ft_split_free_null(envp);
	return (str);
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
	if (w->offset == 0 || *vector_gets(&v) == ' ')
		wordexp_append_arg(w, fields[0]);
	else
		wordexp_join_arg(w, fields[0]);
	i = 1;
	while (i < ft_strslen(fields))
		wordexp_append_arg(w, fields[i++]);
	free(fields);
	if (ft_last_chr(vector_gets(&v)) == ' ')
		w->offset = 0;
	free(v.data);
	return (word);
}
