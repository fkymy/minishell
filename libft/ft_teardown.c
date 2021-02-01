/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_teardown.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 10:52:59 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/31 14:42:28 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_teardown(char **res, int allocations)
{
	int	i;

	i = 0;
	while (i < allocations)
		free(res[i]);
	free(res);
	return (NULL);
}
