/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_join_chr.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 17:23:20 by tayamamo          #+#    #+#             */
/*   Updated: 2021/02/08 17:23:31 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_split_join_chr(char **split, char chr)
{
	char	*ret;
	int		i;

	i = -1;
	ret = ft_strdup("");
	while (split[++i])
		ret = ft_strjoin_chr_free(ret, ft_strdup(split[i]), chr);
	ft_split_free_null(split);
	return (ret);
}
