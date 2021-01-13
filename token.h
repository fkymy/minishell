/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 21:52:14 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/11 21:53:23 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# define TOKEN_WORD 0
# define TOKEN_REDIRECTION 1
# define TOKEN_SEPARATOR 2
# define TOKEN_BACKGROUND 3
# define TOKEN_PIPE 4
# define TOKEN_AND 5
# define TOKEN_OR 6
# define TOKEN_OTHER -1

int		token_isop(int t);

int		token_ispipe(int t);

char	*get_next_token(char *str, int *type, char **token);

#endif
