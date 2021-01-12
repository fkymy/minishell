/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/27 17:17:17 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/12 18:07:53 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_will_overflow(unsigned long n, int next_digit)
{
	if (n > (LONG_MAX) / 10)
		return (1);
	if (n == (LONG_MAX / 10) && next_digit > (LONG_MAX % 10))
		return (1);
	return (0);
}

static int	ft_strtonbr(const char *str, int sign)
{
	unsigned long	n;
	int				d;

	n = 0;
	while (ft_isdigit(*str))
	{
		d = *str - '0';
		if (ft_will_overflow(n, d))
			return (sign == 1 ? (int)LONG_MAX : (int)LONG_MIN);
		n = n * 10 + d;
		str++;
	}
	return ((int)n * sign);
}

int			ft_atoi(const char *str)
{
	int		sign;

	while (ft_isspace(*str))
		str++;
	sign = (*str == '-') ? -1 : 1;
	if (*str == '+' || *str == '-')
		str++;
	return (ft_strtonbr(str, sign));
}
