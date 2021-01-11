/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 18:23:41 by tayamamo          #+#    #+#             */
/*   Updated: 2020/11/17 07:05:33 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_null(char **ptr)
{
	size_t	len;
	char	*p;

	if (!ptr || !*ptr)
		return ;
	if (*ptr)
	{
		len = ft_strlen(*ptr);
		p = *ptr;
		while (len--)
			*(p++) = '\0';
		free(*ptr);
		*ptr = NULL;
	}
	return ;
}

static void	set_text(char *text[], char *tmp, const int fd)
{
	free_null(&(text[fd]));
	text[fd] = ft_strdup(tmp);
	free_null(&tmp);
}

static int	read_file(char *text[], const int fd)
{
	char	*buf;
	ssize_t	size;
	char	*tmp;

	if (!(buf = malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return (-1);
	size = read(fd, buf, BUFFER_SIZE);
	if (!size)
	{
		free(buf);
		return (0);
	}
	else if (size < 0)
	{
		free(buf);
		return (-1);
	}
	buf[size] = '\0';
	tmp = ft_strjoin(text[fd], buf);
	free(buf);
	set_text(text, tmp, fd);
	if (!(text[fd]))
		return (-1);
	return (1);
}

static int	set_line(char *text[], const int fd, char **line, char *ptr)
{
	char	*tmp;

	if (ft_strlen(text[fd]))
	{
		if (!ft_strchr(text[fd], '\n'))
		{
			*line = ft_strdup(text[fd]);
			free_null(&(text[fd]));
			return (0);
		}
		*ptr = '\0';
		*line = ft_strdup(text[fd]);
		tmp = ft_strdup(ptr + 1);
		set_text(text, tmp, fd);
	}
	else
	{
		*line = ft_strdup(text[fd]);
		free_null(&(text[fd]));
		return (0);
	}
	return (1);
}

int			get_next_line(int fd, char **line)
{
	static char	*text[1024];
	char		buf[1];
	char		*ptr;
	int			ret;

	if (fd < 0 || fd >= 1024 || !line || read(fd, buf, 0) < 0
			|| BUFFER_SIZE < 1)
		return (-1);
	if (!(text[fd]))
		if (!(text[fd] = ft_strdup("")))
			return (-1);
	while (!(ptr = ft_strchr(text[fd], '\n')))
	{
		ret = read_file(text, fd);
		if (!ret)
			break ;
		else if (ret == -1)
		{
			free_null(&(text[fd]));
			return (-1);
		}
	}
	return (set_line(text, fd, line, ptr));
}
