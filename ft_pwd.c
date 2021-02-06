/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 17:26:51 by tayamamo          #+#    #+#             */
/*   Updated: 2021/02/06 02:05:51 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/errno.h>
#include "minishell.h"
#include "libft/libft.h"

char	**split_skip_str(char **split, char *str, int *cnt)
{
	char	**ret;
	int		i;
	int		j;

	i = -1;
	*cnt = 0;
	while (split[++i])
	{
		if (ft_strcmp(split[i], str) == 0)
			(*cnt)++;
	}
	if ((ret = malloc(sizeof(char *) * (i - (*cnt) + 1))) == NULL)
		die(strerror(errno));
	i = -1;
	j = 0;
	while (split[++i])
	{
		if (ft_strcmp(split[i], str) != 0)
			ret[j++] = ft_strdup(split[i]);
	}
	ret[j] = NULL;
	ft_split_free_null(split);
	free(str);
	return (ret);
}

char	*split_join_cnt(char **split, int *cnt)
{
	char	*ret;
	int		i;
	int		j;
	char	*tmp;

	i = -1;
	while (split[++i])
		;
	j = i - *cnt;
	i = 0;
	ret = ft_strdup("");
	while (i < j)
	{
		tmp = ft_strdup(split[i]);
		ret = ft_strjoin_chr(ret, tmp, '/');
		free(tmp);
		i++;
	}
	ft_split_free_null(split);
	return (ret);
}

char	*format_pwd(char *pwd, char *argv)
{
	char	*path;
	char	**tmp;
	int		slasla;
	int		cnt;

	slasla = is_path_slasla(pwd);
	path = ft_strjoin_chr(pwd, argv, '/');
	free(pwd);
	free(argv);
	tmp = split_skip_str(ft_split(path, '/'), ft_strdup("."), &cnt);
	free(path);
	tmp = split_skip_str(tmp, ft_strdup(".."), &cnt);
	path = split_join_cnt(tmp, &cnt);
	if (slasla == 1)
	{
		if (path[0] == '\0')
			path = ft_strdup("//");
		else
			path = ft_strjoin("/", path);
	}
	if (path[0] == '\0' && slasla == 0)
		path = ft_strdup("/");
	return (path);
}

char	*get_pwd(void)
{
	if (env_get(g_env, "PWD") == NULL)
		return ("");
	return (env_get(g_env, "PWD")->value);
}

int		ft_pwd(void)
{
	ft_putstr_fd(g_pwd, 1);
	ft_putstr_fd("\n", 1);
	return (0);
}
