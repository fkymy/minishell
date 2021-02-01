/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 18:27:39 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/01 16:06:13 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* builtins */
int				ft_exit(char *argv[]);
int				ft_export(char *argv[]);
int				ft_unset(char *argv[]);
int				ft_env(char *argv[]);

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

/* env.c */
typedef struct		s_env {
	struct s_env	*next;
	char			*name;
	char			*value;
}					t_env;

extern t_env	*g_env;
t_env			*env_init(void);
t_env			*env_new(char *str);
t_env			*env_get(t_env *e, char *name);
void			env_set(t_env **e, t_env *new);
void			env_unset(t_env **ep, char *name);
char			**env_make_envp(t_env *e, int isexport);
char			*env_split_name(char *str);
void			env_print(char *str, int quote);

char			*env_join_name_value(t_env *env);

/* main.c */
extern int		g_exit_status;
void			die(char *msg);

/* signal.c */
volatile sig_atomic_t	g_interrupt;
void					handler(int signum);
void					set_signal_handler(void (*func)(int));


#endif
