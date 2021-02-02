/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stradd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 14:02:58 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/01 14:03:13 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_stradd(char **current, char *new)
{
	if (*current == NULL)
		*current = ft_strdup(new);
	else
		*current = ft_strjoin_free(*current, ft_strdup(new));
}
