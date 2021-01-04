/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/03 16:24:43 by yufukuya          #+#    #+#             */
/*   Updated: 2020/07/23 20:04:01 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*str1;
	const unsigned char	*str2;
	size_t				i;
	int					delta;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	delta = 0;
	while (i < n)
	{
		if (*(str1 + i) != *(str2 + i))
		{
			delta = *(str1 + i) - *(str2 + i);
			return (delta);
		}
		i++;
	}
	return (0);
}
