/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 18:27:39 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/29 18:20:28 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* get_next_commandline.c */
int				get_next_commandline(int fd, char **line);

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
void			command_clear_args(char **argv);
void			command_lstclear(t_command **c);

/* vector.c */
typedef struct	s_vector
{
	char	*data;
	size_t	size;
	size_t	capacity;
	int		error;
}				t_vector;

void			vector_initialize(t_vector *v);
void			vector_append(t_vector *v, int c);
void			vector_appends(t_vector *v, char *s);
char			*vector_gets(t_vector *v);
void			vector_free(t_vector *v);

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

/* parse.c */
# define OP_PIPE 2
# define OP_AND 3
# define OP_OR 4
# define OP_SEPARATOR 5
# define OP_OTHER -1
int				parse(char *commandline, t_command **c);
int				isredir(char *str);

/* redir.c */
char			**handle_redir(char **argv);

/* wordexp.c */
typedef struct	s_wordexp
{
	size_t 		wordc;
	char		**wordv;
	size_t		offset;
}				t_wordexp;

int				wordexp(char *word, t_wordexp *w);
char			**wordexp_wrap(char *word);

/* main.c */
extern int		g_exit_status;
void			die(char *msg);

/* signal.c */
volatile sig_atomic_t	g_interrupt; // volatile?
void					handler(int signum);
void					set_signal_handler(void (*func)(int));


#endif
