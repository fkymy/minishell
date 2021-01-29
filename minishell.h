/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 18:27:39 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/29 12:21:50 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"

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

/* dict.c */
typedef struct	s_dict {
	char	*key;
	char	*val;
}				t_dict;

void			dict_update_val(t_list *dict, char *key, char *val);
char			*dict_get_val(t_list *dict, char *key);
int				dict_find_key(t_list *dict, char *key);
t_dict			*dict_make_new(char *key, char *val);

/* env.c */
void			env_update_shlvl(t_list *env);
t_list			*env_initialize(void);

/* env_update.c */
void			update_shlvl(t_list *env);
void			update_pwd(t_list *env);

/* main.c */
extern int		g_exit_status;
void			die(char *msg);

#endif
