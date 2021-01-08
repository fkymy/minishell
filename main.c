/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 18:44:13 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/08 10:36:34 by yufukuya         ###   ########.fr       */
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


/* Utilities */

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

/* void Log(char *fmt, ...) */
/* { */
/*  */
/* } */


/* Tokenizer */

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
		if (str[1] == '>')
		{
			append_to_token(str[1], &tokenizer);
			++str;
		}
		++str;
	}
	else if (isandor(str))
	{
		*type = *str == '&' ? TOKEN_AND : TOKEN_OR;
		append_to_token(*str, &tokenizer);
		append_to_token(str[1], &tokenizer);
		str += 2;
	}
	else if (isoperator(*str))
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
				++str;
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


/* Command */

typedef struct	s_command
{
	int					argc;
	char				**argv;
	pid_t				pid;

	struct s_command	*next;
	int					op; // Connecting op & ; | && ||
}				t_command;

t_command	*command_new(void)
{
	t_command	*c;

	if (!(c = malloc(sizeof(t_command))))
		return (NULL);
	c->argc = 0;
	c->argv = NULL;
	c->pid = -1;
	c->next = NULL;
	c->op = -1;
	return (c);
}

void		command_clear(t_command *c)
{
	int	i;

	i = 0;
	while (i < c->argc)
		free(c->argv[i++]);
	free(c);
}

void		command_append_arg(t_command *c, char *word)
{
	char	**new_argv;
	int		i;

	new_argv = malloc(sizeof(char *) * c->argc + 2);
	i = 0;
	while (i < c->argc)
	{
		new_argv[i] = c->argv[i];
		i++;
	}
	free(c->argv);
	c->argv = new_argv;
	c->argv[c->argc] = word;
	c->argv[c->argc + 1] = NULL;
	++c->argc;
}

int			command_isbackground(t_command *c)
{
	while (c->op != TOKEN_SEPARATOR && c->op != TOKEN_BACKGROUND)
		c = c->next;
	return (c-> op == TOKEN_BACKGROUND);
}

int			command_isleftsidepipe(t_command *current)
{
	t_command *next;

	if (!current->next)
		return (0);
	next = current->next;
	if (next->op == TOKEN_PIPE)
		return (1);
	return (0);
}

int			command_isrightsidepipe(t_command *head, t_command *current)
{
	while (head->next)
	{
		if (head->op == TOKEN_PIPE && head->next == current)
			return (1);
		head = head->next;
	}
	return (0);
}



/* parse */

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
