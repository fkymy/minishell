/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/05 10:43:40 by yufukuya          #+#    #+#             */
/*   Updated: 2020/07/05 11:57:35 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_swap(char *a, char *b)
{
	char c;

	c = *a;
	*a = *b;
	*b = c;
}

static void	ft_reverse(char *s)
{
	int i;
	int j;

	i = 0;
	j = ft_strlen(s) - 1;
	while (i < j)
		ft_swap(&s[i++], &s[j--]);
}

char		*ft_itoa(int n)
{
	char	s[12];
	int		i;
	int		sign;

	if (n == INT_MIN)
		return (ft_strdup("-2147483648"));
	if ((sign = n) < 0)
		n = -n;
	i = 0;
	s[i++] = n % 10 + '0';
	while ((n /= 10) > 0)
		s[i++] = n % 10 + '0';
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	ft_reverse(s);
	return (ft_strdup(s));
}
