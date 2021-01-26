/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 21:30:31 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/25 20:32:02 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft/libft.h"
#include "minishell.h"

void	vector_initialize(t_vector *v)
{
	v->data = NULL;
	v->size = 0;
	v->capacity = 0;
	v->error = 0;
}

void	vector_append(t_vector *v, int c)
{
	size_t	new_capacity;
	char	*new_data;

	if (v->error)
		return ;
	if (v->size == v->capacity)
	{
		new_capacity = v->capacity ? v->capacity * 2 : 8;
		if (!(new_data = malloc(new_capacity)))
		{
			v->error = 1;
			return ;
		}
		ft_memcpy(new_data, v->data, v->size);
		free(v->data);
		v->data = new_data;
		v->capacity = new_capacity;
	}
	v->data[v->size] = c;
	++v->size;
}

void	vector_appends(t_vector *v, char *s)
{
	while (*s)
		vector_append(v, *s++);
}

char	*vector_gets(t_vector *v)
{
	if (v->size == 0)
		vector_append(v, '\0');
	else if (v->data[v->size - 1] != '\0')
		vector_append(v, '\0');
	if (v->error)
		return (NULL);
	return (v->data);
}

void	vector_free(t_vector *v)
{
	free(v->data);
	v->data = NULL;
}

