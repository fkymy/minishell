/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 19:39:32 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/11 19:40:18 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

typedef struct	s_command
{
	struct s_command	*next;
	int					argc;
	char				**argv;
	int					op;      // Connecting operators are & ; | && ||
	pid_t				pid;
}				t_command;

t_command		*command_new(void);
void			command_clear(t_command *c);
void			command_append_arg(t_command *c, char *word);

#endif
