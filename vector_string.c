/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 21:30:31 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/13 17:34:05 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "vector_string.h"
#include "libft/libft.h"

void	vector_initialize(t_vector_string *v)
{
	v->data = NULL;
	v->size = 0;
	v->capacity = 0;
}

void	vector_append(t_vector_string *v, int c)
{
	size_t	new_capacity;
	char	*new_data;

	if (v->size == v->capacity)
	{
		new_capacity = v->capacity ? v->capacity * 2 : 8;
		new_data = malloc(new_capacity);
		ft_memcpy(new_data, v->data, v->size);
		free(v->data);
		v->data = new_data;
		v->capacity = new_capacity;
	}
	v->data[v->size] = c;
	++v->size;
}

