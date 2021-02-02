/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 14:35:30 by yufukuya          #+#    #+#             */
/*   Updated: 2021/01/31 14:35:33 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	swap_elements(char **lhs, char **rhs)
{
	char	*tmp;

	tmp = *lhs;
	*lhs = *rhs;
	*rhs = tmp;
}

void		ft_strsort(char *arr[], int size, int (*cmp)(char *, char *))
{
	int i;
	int j;
	int did_swap;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		did_swap = 0;
		while (j < size - 1 - i)
		{
			if (cmp(arr[j], arr[j + 1]) > 0)
			{
				swap_elements(&arr[j], &arr[j + 1]);
				did_swap = 1;
			}
			j++;
		}
		if (!did_swap)
			return ;
		i++;
	}
}
