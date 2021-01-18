/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 19:39:32 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/18 08:45:01 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

typedef struct	s_command
{
	struct s_command	*next;
	int					argc;
	char				**argv;
	int					op;
}				t_command;

t_command		*command_new(void);
int				command_append_arg(t_command *c, char *word);
void			command_clear(t_command *c);
void			command_lstclear(t_command **c);

#endif
