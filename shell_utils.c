/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 18:15:11 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/03 12:37:19 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int	isshellspecial(int c)
{
	return (c == '<' || c == '>' ||
			c == '&' || c == '|' || c == ';');
}

int	isredir(char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, ">") == 0)
		return (1);
	if (ft_strcmp(str, "<") == 0)
		return (1);
	if (ft_strcmp(str, ">>") == 0)
		return (1);
	return (0);
}
