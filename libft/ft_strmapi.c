/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/05 11:49:30 by yufukuya          #+#    #+#             */
/*   Updated: 2020/07/05 12:09:59 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*new;
	unsigned int	i;

	if (s == NULL)
		return (NULL);
	if (!(new = ft_strdup(s)))
		return (NULL);
	if (f == NULL)
		return (new);
	i = -1;
	while (new[++i])
		new[i] = f(i, s[i]);
	return (new);
}
