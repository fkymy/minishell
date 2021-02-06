/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_chr_free.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 15:26:24 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/06 15:26:44 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_chr_free(char *s1, char *s2, char c)
{
	char	*res;

	res = ft_strjoin_chr(s1, s2, c);
	free(s1);
	free(s2);
	return (res);
}
