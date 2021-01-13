/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_string.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 21:27:32 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/11 21:28:26 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_STRING_H
# define VECTOR_STRING_H

typedef struct	s_vector_string
{
	char	*data;
	size_t	size;
	size_t	capacity;
}				t_vector_string;

void			vector_initialize(t_vector_string *v);
void			vector_append(t_vector_string *v, int c);

#endif
