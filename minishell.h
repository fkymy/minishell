/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 18:27:39 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/09 00:01:28 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <assert.h>
# include <ctype.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>

typedef struct			s_command
{
	struct s_command	*next;
	int					argc;
	char				**argv;
	int					op;
	pid_t				pid;
}						t_command;

typedef struct			s_vector
{
	char				*data;
	size_t				size;
	size_t				capacity;
	int					error;
}						t_vector;

typedef struct			s_wordexp
{
	size_t				wordc;
	char				**wordv;
	size_t				offset;
}						t_wordexp;

typedef struct			s_env
{
	struct s_env		*next;
	char				*name;
	char				*value;
}						t_env;

extern t_env			*g_env;
extern int				g_exit_status;
volatile int			g_interrupt;
extern char				*g_pwd;

int						ft_echo(char *argv[]);
int						ft_cd(char *argv[]);
void					puterr_cd_getcwd(void);
int						puterr_cd(char *file);
void					puterr_cd_getcwd_dot(void);
void					check_symbolic_link(char *argv);
int						is_path_slasla(char *path);
int						is_path_sla(char *path);
int						ft_pwd(void);
char					*get_pwd(void);
char					*format_pwd(char *pwd, char *argv);
int						ft_exit(char *argv[]);
int						ft_export(char *argv[]);
int						ft_unset(char *argv[]);
int						ft_env(char *argv[]);

t_command				*command_new(void);
int						command_append_arg(t_command *c, char *word);
void					command_clear_args(char **argv);
void					command_lstclear(t_command **c);

pid_t					start_command(char *argv[], int ispipe,
										int haspipe, int lastpipe[2]);
void					run_list(t_command *c);
char					**process_words(char *argv[]);

void					setup_pipe(int ispipe, int haspipe,
									int newpipe[2], int lastpipe[2]);
void					cleanup_pipe(int haspipe, int lastpipe[2]);
t_command				*do_pipeline(t_command *c);
void					wait_pipeine(pid_t pid);

int						is_builtin(char *word, int expand);
int						exec_builtin(char *argv[]);
int						exec_builtin_parent(t_command *c);

int						get_next_commandline(int fd, char **line);

void					vector_initialize(t_vector *v);
void					vector_append(t_vector *v, int c);
void					vector_appends(t_vector *v, char *s);
char					*vector_gets(t_vector *v);
void					vector_free(t_vector *v);

# define TOKEN_WORD 0
# define TOKEN_REDIRECTION 1
# define TOKEN_PIPE 2
# define TOKEN_AND 3
# define TOKEN_OR 4
# define TOKEN_SEPARATOR 5
# define TOKEN_OTHER -1

char					*get_next_token(char *str, int *type, char **token);

# define OP_PIPE 2
# define OP_AND 3
# define OP_OR 4
# define OP_SEPARATOR 5
# define OP_OTHER -1

int						parse(char *commandline, t_command *c);

char					**process_redir(char **argv, int *in, int *out);

int						wordexp(char *word, t_wordexp *w);
char					**wordexp_wrap(char *word);

void					wordexp_join_arg(t_wordexp *w, char *s);
int						wordexp_append_arg(t_wordexp *w, char *word);

char					*shift_quotes(char *word, t_wordexp *w);
char					*shift_expansion(char *word, t_wordexp *w);
char					*expand(char *str, t_vector *v);

t_env					*env_init(void);
t_env					*env_new(char *str);
t_env					*env_get(t_env *e, char *name);
void					env_set(t_env **e, t_env *new);
void					env_unset(t_env **ep, char *name);
char					**env_make_envp(t_env *e, int isexport);
char					*env_split_name(char *str);
char					*env_split_value(char *str);
int						env_size(t_env *env);
void					env_free(t_env *e);

void					env_print(char *str, int quote);
t_env					*env_make_new(char *name, char *value);

char					*env_join_name_value(t_env *env);

char					*build_path(char *cmd);
int						is_cmd_dir(char *path2cmd);
int						is_cmd_permitted(char *path2cmd);

char					**process_words(char *argv[]);

void					handler(int signum);
void					set_signal_handler(void (*func)(int));

int						isshellspecial(int c);
int						isredir(char *str);

void					die(char *msg);

#endif
