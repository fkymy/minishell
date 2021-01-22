/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 18:27:39 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/22 19:29:46 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* command.c */
typedef struct	s_command
{
	struct s_command	*next;
	int					argc;
	char				**argv;
	int					op;
	pid_t				pid;
}				t_command;

t_command		*command_new(void);
int				command_append_arg(t_command *c, char *word);
void			command_clear(t_command *c);
void			command_lstclear(t_command **c);

/* vector_string.c */
typedef struct	s_vector_string
{
	char	*data;
	size_t	size;
	size_t	capacity;
	int		error;
}				t_vector_string;

void			vector_initialize(t_vector_string *v);
void			vector_append(t_vector_string *v, int c);
void			vector_free(t_vector_string *v);

/* token.c */
# define TOKEN_WORD 0
# define TOKEN_REDIRECTION 1
# define TOKEN_PIPE 2
# define TOKEN_AND 3
# define TOKEN_OR 4
# define TOKEN_SEPARATOR 5
# define TOKEN_OTHER -1

int				token_isop(int t);
int				token_ispipe(int t);
char			*get_next_token(char *str, int *type, char **token);

/* redir.c */
char			**handle_redir(char **argv);

/* wordexp.c */
void			handle_expansion_and_unquote(t_command *c);

/* main.c */
void			die(char *msg);

#endif
