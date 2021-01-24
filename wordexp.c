/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordexp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 11:42:13 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/24 19:23:38 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft/libft.h"
#include "minishell.h"

/*
** Single quote: Preserve literal value of each characters. Single quote cannot be in single quote.
** Double quote: Preserve literal value of each characters except: backquote, $, \
**   - $ expands variables
**   - \ escapes only if followed by $ " \
**
** Things that happen after parse:
** - redirection: redirect and remove operators and operand from param list
** - expansion: expand parts of command into wordexp-like arguments or splitted argv
**   - variables in expansion is not expanded
**   - quotes in expansion is treated literally
** - unquoting: unquote within same argv[i], expand specials
**
** *quotes token shall be unmodified, not delimited during tokenize
** *$ shall be unmodified, not delimited during tokenize
*/

// Environment variable names used by the utilities in the Shell and Utilities volume of IEEE Std 1003.1-2001 consist solely of uppercase letters, digits, and the '_' (underscore) from the characters defined in Portable Character Set and do not begin with a digit. Other characters may be permitted by an implementation

typedef struct	s_wordexp
{
	size_t 		wordc;
	char		**wordv;
	size_t		offset;
}				t_wordexp;

static void		wordexp_join_arg(t_wordexp *w, char *s)
{
	char *new_arg;

	new_arg = ft_strjoin(w->wordv[w->wordc - 1], s);
	free(w->wordv[w->wordc - 1]);
	w->wordv[w->wordc - 1] = new_arg;
	w->offset = ft_strlen(new_arg);
	free(s);
}

static int		wordexp_append_arg(t_wordexp *w, char *word)
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

char	*expand(char *str, t_vector_string *v)
{
	extern char	**environ;
	char		*envkey;
	int			i;

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
	while (environ[i]
			&& ft_strncmp(environ[i], envkey, ft_strlen(envkey)))
		i++;
	if (environ[i])
		vector_appends(v, environ[i] + ft_strlen(envkey));
	free(envkey);
	return (str);
}

char	*unquote_single(char *str, t_vector_string *v)
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

int		is_double_quote_escapable(int c)
{
	return (c == '$' || c == '\\' || c == '\"');
}

char	*unquote_double(char *str, t_vector_string *v)
{
	int escaped;

	if (*str != '\"')
		return (str);
	++str;
	escaped = 0;
	while (*str != '\"')
	{
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

char	*handle_quotes(t_wordexp *w, char *str)
{
	t_vector_string v;

	vector_initialize(&v);
	if (*str == '\'')
		str = unquote_single(str, &v);
	else if (*str == '\"')
		str = unquote_double(str, &v);
	vector_append(&v, '\0');
	if (w->offset)
		wordexp_join_arg(w, v.data);
	else
		wordexp_append_arg(w, v.data);
	++str;
	return (str);
}

char	*handle_expansion(t_wordexp *w, char *str)
{
	t_vector_string v;
	size_t			i;
	char			**fields;

	vector_initialize(&v);
	if (str[1] == '\"')
		return (handle_quotes(w, str + 1));
	str = expand(str, &v);
	if (v.data == NULL)
		return (str);
	vector_append(&v, '\0');
	fields = ft_split(v.data, ' ');
	free(v.data);
	if (w->offset)
		wordexp_join_arg(w, fields[0]);
	else
		wordexp_append_arg(w, fields[0]);
	i = 1;
	while (i < ft_strslen(fields))
		wordexp_append_arg(w, fields[i]);
	free(fields);
	return (str);
}

char	*handle_word(t_wordexp *w, char *str)
{
	t_vector_string v;

	vector_initialize(&v);
	while (*str && *str != '\'' && *str != '\"' && *str != '$')
	{
		vector_append(&v, *str);
		++str;
	}
	vector_append(&v, '\0');
	if (w->offset)
		wordexp_join_arg(w, v.data);
	else
		wordexp_append_arg(w, v.data);
	return (str);
}

char	**wordexp(char **argv)
{
	t_wordexp	w;
	char		*str;
	int			i;

	w.wordc = 0;
	w.wordv = NULL;
	i = 0;
	while (argv[i])
	{
		w.offset = 0;
		str = argv[i];
		while (*str)
		{
			if ((*str == '\'' || *str == '\"'))
				str = handle_quotes(&w, str);
			else if (*str == '$')
				str = handle_expansion(&w, str);
			else
				str = handle_word(&w, str);
		}
		i++;
	}

	// Temporary
	for (int i = 0; i < (int)w.wordc; i++)
		if (ft_strlen(w.wordv[i]) == 0)
			die("wordv should never be zero");

	command_clear_args(argv);
	return (w.wordv);
}
