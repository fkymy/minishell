/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspaces.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 14:24:04 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/05 14:24:12 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_isspaces(char *s)
{
	if (!s)
		return (0);
	while (*s)
		if (!ft_isspace(*s++))
			return (0);
	return (1);
}
