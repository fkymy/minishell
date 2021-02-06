/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 22:21:32 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/05 23:00:12 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_will_overflow(int sign, unsigned long n, int next_digit)
{
	if (sign == 1)
	{
		if (n > (LONG_MAX / 10))
		{
			return (1);
		}
		if (n == (LONG_MAX / 10) && next_digit > (LONG_MAX % 10))
		{
			return (1);
		}
	}
	else if (sign == -1)
	{
		if (n > (LONG_MAX / 10))
		{
			return (1);
		}
		if (n == (LONG_MAX / 10) && next_digit > 8)
		{
			return (1);
		}
	}
	return (0);
}

static long	ft_strtonbr(const char *str, int sign)
{
	unsigned long	n;
	int				d;

	n = 0;
	while (ft_isdigit(*str))
	{
		d = *str - '0';
		if (ft_will_overflow(sign, n, d))
			return (-1);
		n = n * 10 + d;
		str++;
	}
	return (n * sign);
}

long		ft_atol(const char *str)
{
	int		sign;

	while (ft_isspace(*str))
		str++;
	sign = (*str == '-') ? -1 : 1;
	if (*str == '+' || *str == '-')
		str++;
	return (ft_strtonbr(str, sign));
}
