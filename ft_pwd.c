/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 17:26:51 by tayamamo          #+#    #+#             */
/*   Updated: 2021/02/08 18:08:00 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/errno.h>
#include "minishell.h"
#include "libft/libft.h"

char	*is_str_path(char **split, int *total)
{
	char	*ok;
	int		i;
	int		cnt;

	i = -1;
	while (split[++i])
		;
	if ((ok = ft_calloc(1, sizeof(char) * i)) == NULL)
		return (NULL);
	*total = i;
	cnt = 0;
	while (--i >= 0)
	{
		if (ft_strcmp(split[i], "..") == 0)
			cnt++;
		else
		{
			if (cnt == 0)
				ok[i] = 1;
			else
				cnt--;
		}
	}
	return (ok);
}

char	**split_format_dotdot(char **split)
{
	char	**ret;
	int		i;
	char	*ok;
	int		cnt;
	int		total;

	if ((ok = is_str_path(split, &total)) == NULL)
		die(strerror(errno));
	i = -1;
	cnt = 0;
	while (++i < total)
		if (ok[i])
			cnt++;
	if ((ret = malloc(sizeof(char *) * (cnt + 1))) == NULL)
		return (NULL);
	i = -1;
	cnt = 0;
	while (split[++i])
		if (ok[i])
			ret[cnt++] = ft_strdup(split[i]);
	ret[cnt] = NULL;
	ft_split_free_null(split);
	free(ok);
	return (ret);
}

char	*format_pwd(char *pwd, char *argv)
{
	char	*path;
	char	**tmp;
	int		slasla;

	slasla = is_path_slasla(pwd);
	path = ft_strjoin_chr(pwd, argv, '/');
	free(pwd);
	free(argv);
	tmp = ft_split_skip_str(ft_split(path, '/'), ft_strdup("."));
	free(path);
	tmp = split_format_dotdot(tmp);
	path = ft_split_join_chr(tmp, '/');
	if (slasla == 1)
	{
		if (path[0] == '\0')
			path = ft_strjoin_chr_free(ft_strdup("//"), path, '\0');
		else
			path = ft_strjoin_free(ft_strdup("/"), path);
	}
	if (path[0] == '\0' && slasla == 0)
		path = ft_strjoin_chr_free(ft_strdup("/"), path, '\0');
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
