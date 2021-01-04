/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/04 17:00:11 by yufukuya          #+#    #+#             */
/*   Updated: 2020/07/07 20:39:54 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t catsize;
	size_t dstlen;
	size_t srclen;
	size_t i;

	catsize = 0;
	while (catsize < dstsize && dst[catsize])
		catsize++;
	dstlen = ft_strlen(dst);
	srclen = ft_strlen(src);
	i = 0;
	while (i < srclen && dstlen + i + 1 < dstsize)
	{
		dst[dstlen + i] = src[i];
		i++;
	}
	dst[dstlen + i] = '\0';
	return (catsize + srclen);
}
