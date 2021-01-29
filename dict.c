/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dict.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 11:59:06 by tayamamo          #+#    #+#             */
/*   Updated: 2021/01/29 21:31:10 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

void	dict_update_val(t_list *dict, char *key, char *val)
{
	t_list	*tmp;

	tmp = dict;
	while (tmp)
	{
		if (ft_strcmp(((t_dict *)tmp->content)->key, key) == 0)
		{
			free(((t_dict *)tmp->content)->val);
			((t_dict *)tmp->content)->val = val;
		}
		tmp = tmp->next;
	}
}

char	*dict_get_val(t_list *dict, char *key)
{
	t_list	*tmp;

	tmp = dict;
	while (tmp)
	{
		if (ft_strcmp(((t_dict *)tmp->content)->key, key) == 0)
			return (((t_dict *)tmp->content)->val);
		tmp = tmp->next;
	}
	return (NULL);
}

t_dict	*dict_make_new(char *key, char *val)
{
	t_dict	*new;

	if ((new = malloc(sizeof(t_dict))) == NULL)
		die("malloc failed\n");
	new->key = key;
	new->val = val;
	return (new);
}
