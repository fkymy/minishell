/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_skip_str.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 17:32:20 by tayamamo          #+#    #+#             */
/*   Updated: 2021/02/08 17:32:35 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_split_skip_str(char **split, char *str)
{
	char	**ret;
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (split[++i])
		if (ft_strcmp(split[i], str) == 0)
			j++;
	if ((ret = malloc(sizeof(char *) * (i - j + 1))) == NULL)
		return (NULL);
	i = -1;
	j = 0;
	while (split[++i])
		if (ft_strcmp(split[i], str) != 0)
			ret[j++] = ft_strdup(split[i]);
	ret[j] = NULL;
	ft_split_free_null(split);
	free(str);
	return (ret);
}
