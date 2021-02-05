/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yufukuya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 16:48:48 by yufukuya          #+#    #+#             */
/*   Updated: 2021/02/05 16:54:16 by yufukuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <sys/stat.h>

#include "libft/libft.h"
#include "minishell.h"

typedef struct stat	t_stat;

char	*build_absolute_path(char *cmd)
{
	char	*cwd;
	char	*path;

	if (*cmd == '/')
		return (ft_strdup(cmd));
	if (!(cwd = getcwd(NULL, 0)))
		die(strerror(errno));
	if (!(path = ft_strjoin_chr(cwd, cmd, '/')))
		die(strerror(errno));
	free(cwd);
	return (path);
}

char	*check_path(char *cmd, char *path2cmd)
{
	t_stat	stat_buf;

	if (is_cmd_dir(path2cmd))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": is a directory\n", 2);
		exit(126);
	}
	if (stat(path2cmd, &stat_buf) == 0)
	{
		if (!is_cmd_permitted(path2cmd))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(126);
		}
		return (path2cmd);
	}
	return (NULL);
}

char	*find_from_env(char *cmd, char **paths)
{
	char	*path2cmd;
	int		i;

	i = 0;
	while (paths[i])
	{
		if ((path2cmd = ft_strjoin_chr(paths[i], cmd, '/')) == NULL)
			die(strerror(errno));
		if (check_path(cmd, path2cmd))
			return (path2cmd);
		ft_free_null(&path2cmd);
		i++;
	}
	return (NULL);
}

char	*build_path(char *cmd)
{
	char	*path2cmd;
	char	**paths;
	t_env	*pathenv;

	if (!cmd || ft_strlen(cmd) == 0)
		return (NULL);
	if (ft_strcmp(cmd, ".") == 0 || ft_strcmp(cmd, "..") == 0)
		return (NULL);
	paths = NULL;
	pathenv = env_get(g_env, "PATH");
	if (pathenv && pathenv->value)
		if (!(paths = ft_split(pathenv->value, ':')))
			die("failed to split path");
	if (ft_strchr(cmd, '/') || ft_strslen(paths) == 0)
	{
		path2cmd = build_absolute_path(cmd);
		return (check_path(cmd, path2cmd));
	}
	path2cmd = find_from_env(cmd, paths);
	ft_split_free_null(paths);
	return (path2cmd);
}
