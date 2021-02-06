/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 16:52:55 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/05 16:53:11 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

typedef struct stat	t_stat;

int		is_cmd_dir(char *path2cmd)
{
	t_stat	stat_buf;

	if (stat(path2cmd, &stat_buf) == 0)
		if ((stat_buf.st_mode & S_IFDIR) == S_IFDIR)
			return (1);
	return (0);
}

int		is_cmd_permitted(char *path2cmd)
{
	t_stat	stat_buf;

	if (stat(path2cmd, &stat_buf) == 0)
		if ((stat_buf.st_mode & S_IXUSR) == S_IXUSR)
			return (1);
	return (0);
}
