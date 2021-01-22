/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordexp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 11:42:13 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/22 19:28:28 by yufukuya         ###   ########.fr       */
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

typedef struct	wordexp {
	size_t 		wordc;
	char		**wordv;
	size_t		offset;
}				t_wordexp;

static void		command_clear_argv(t_command *c)
{
	int	i;

	if (c == NULL)
		return ;
	i = 0;
	while (i < c->argc)
		free(c->argv[i++]);
	free(c->argv);
}

int			wordexp_append_arg(struct wordexp *w, char *word)
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
	return (0);
}

char	*expand(char *str, t_vector_string *v)
{
	extern char **environ;

	if (*str != '$')
		return (str);

	if (!ft_isalpha(str[1]) && str[1] != '_')
	{
		vector_append(v, *str);
		return (str + 1);
	}
	++str;

	int len = 0;
	while (ft_isalnum(str[len]) || str[len] == '_')
		++len;
	char *envvar = ft_substr(str, 0, len + 1);
	envvar[len] = '=';
	str += len;

	int i = 0;
	while (environ[i] && ft_strncmp(environ[i], envvar, ft_strlen(envvar)))
		i++;

	if (environ[i])
	{
		int j = ft_strlen(envvar);
		while (environ[i][j])
		{
			vector_append(v, environ[i][j]);
			j++;
		}
	}
	free(envvar);
	return (str);
}

char	*handle_quotes(t_wordexp *w, char *str, int quoted)
{
	t_vector_string v;

	vector_initialize(&v);

	if (quoted == '\'')
	{
		while (*str != quoted)
		{
			vector_append(&v, *str);
			++str;
		}
	}
	else if (quoted == '\"')
	{
		int escaped = 0;
		while (*str != quoted)
		{
			if (*str == '\\' && (str[1] == '$' || str[1] == '\\' || str[1] == '\"'))
			{
				escaped = 1;
				++str;
			}
			if (*str == '$' && !escaped)
			{
				str = expand(str, &v);
			}
			else
			{
				vector_append(&v, *str);
				++str;
			}
		}
	}
	vector_append(&v, '\0');
	if (w->offset)
	{
		char *tmp = ft_strjoin(w->wordv[w->wordc - 1], v.data);
		free(w->wordv[w->wordc - 1]);
		w->wordv[w->wordc - 1] = tmp;
		free(v.data);
		w->offset += v.size;
	}
	else
	{
		wordexp_append_arg(w, v.data);
		w->offset += v.size;
	}
	++str;
	return (str);
}

char	*handle_expansion(t_wordexp *w, char *str, int quoted)
{
	// same as above, except it will split
	(void)w;
	(void)quoted;
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
	{
		char *tmp = ft_strjoin(w->wordv[w->wordc - 1], v.data);
		free(w->wordv[w->wordc - 1]);
		w->wordv[w->wordc - 1] = tmp;
		free(v.data);
		w->offset += v.size;
	}
	else
	{
		wordexp_append_arg(w, v.data);
		w->offset += v.size;
	}
	return (str);
}

void	handle_expansion_and_unquote(t_command *c)
{
	t_wordexp w;
	w.wordc = 0;
	w.wordv = NULL;

	int i = 0;
	while (i < c->argc)
	{
		/* printf("handling c->argv[%d]: %s\n", i, c->argv[i]); */
		// echo $USER aaa$USER"bbb 'ccc' """"$USER "$USERddd

		w.offset = 0;
		int quoted = 0;
		char *str = c->argv[i];
		while (*str)
		{
			if ((*str == '\'' || *str == '\"') && !quoted)
			{
				quoted = *str;
				++str;
				str = handle_quotes(&w, str, quoted);
				quoted = 0;
			}
			else if (*str == '$')
			{
				str = handle_expansion(&w, str, quoted);
			}
			else
			{
				str = handle_word(&w, str);
			}
		}
		i++;
	}

	printf("new wordv:\n");
	for (int i = 0; i < (int)w.wordc; i++)
	{
		printf("w.wordv[%d]: %s\n", i, w.wordv[i]);
	}

	command_clear_argv(c);
	c->argv = w.wordv;
}
