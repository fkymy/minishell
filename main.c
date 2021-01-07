/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 18:44:13 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/07 19:23:03 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <ctype.h>

# define TOKEN_WORD 0
# define TOKEN_REDIRECTION 1
# define TOKEN_SEPARATOR 2
# define TOKEN_BACKGROUND 3
# define TOKEN_PIPE 4
# define TOKEN_AND 5
# define TOKEN_OR 6
# define TOKEN_LPAREN 7
# define TOKEN_RPAREN 8
# define TOKEN_OTHER -1

int		ft_isspace(int c)
{
	return (c == '\t' || c == '\n' || c == '\v' ||
			c == '\f' || c == '\r' || c == ' ');
}

int		isoperator(int c)
{
	return (c == '<' || c == '>' || c == '&' || c == '|' ||
			c == ';' || c == '(' || c == ')' || c == '#');
}

int		isredirect(int c)
{
	return (c == '<' || c == '>');
}

int		isandor(char *s)
{
	return ((*s == '&' || *s == '|') && s[1] == *s);
}

typedef struct	s_tokenizer
{
	char	*token;
	int		len;
	int		cap;
}				t_tokenizer;


void	append_to_token(int c, t_tokenizer *t)
{
	int		new_cap;
	char	*new_token;

	if (t->len == t->cap)
	{
		new_cap = t->cap ? t->cap * 2 : 8;
		new_token = malloc(new_cap);
		ft_memcpy(new_token, t->token, t->len);
		free(t->token);
		t->token = new_token;
		t->cap += new_cap;
	}
	t->token[t->len++] = c;
}

char	*tokenize(char *str, int *type, char **token)
{
	t_tokenizer tokenizer;

	ft_memset(&tokenizer, 0, sizeof(tokenizer));

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
		append_to_token(*str, &tokenizer);
		++str;
		if (str[1] == '>')
		{
			append_to_token(str[1], &tokenizer);
			++str;
		}
	}
	else if (isandor(str))
	{
		*type = *str == '&' ? TOKEN_AND : TOKEN_OR;
		append_to_token(*str, &tokenizer);
		append_to_token(str[1], &tokenizer);
		str += 2;
	}
	else if (isspecial(*str))
	{
		switch (*str) {
		case ';': *type = TOKEN_SEPARATOR;   break;
		case '&': *type = TOKEN_BACKGROUND; break;
		case '|': *type = TOKEN_PIPE;       break;
		case '(': *type = TOKEN_LPAREN;     break;
		case ')': *type = TOKEN_RPAREN;     break;
		default:  *type = TOKEN_OTHER;      break;
		}
		append_to_token(*str, &tokenizer);
		++str;
	}
	else
	{
		*type = TOKEN_WORD;
		int quoted = 0;
		while ((*str && quoted)
				|| (*str && !ft_isspace(*str)
					&& !isoperator(*str)))
		{
			if ((*str == '\"' || *str == '\'') && !quoted)
				quoted = *str;
			else if (*str == quoted)
				quoted = 0;
			else if (*str == '\\' && str[1] != '\0' && quoted != '\'')
			{
				append_to_token(str[1], &tokenizer);
				str++;
			}
			else
				append_to_token(*str, &tokenizer);
			++str;
		}
	}

	append_to_token('\0', &tokenizer);
	*token = tokenizer.token;
	return (str);
}

void	parse_line(char* s)
{
	int		type;
	char	*token;

	// Test Tokenizer
	while ((s = tokenize(s, &type, &token)) != NULL) {
		printf("[%d] %s\n", type, token);
		free(token);
	}
}

int		main(int argc, char *argv[])
{
	(void)argc;
	// Usage: ./minishell 'some commandline here'
	printf("Testing parse for string \"%s\"\n", argv[1]);
	parse_line(argv[1]);

	exit(0);
}
