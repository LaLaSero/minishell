/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:37:53 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/20 14:00:01 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/get_next_line.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	char	*p;

	i = 0;
	if (s == NULL)
		return (NULL);
	p = (char *)s;
	if (c == '\0')
		return (p + ft_strlen(p));
	while (p[i] != '\0')
	{
		if (p[i] == (char)c)
			return (p + i);
		i++;
	}
	return (NULL);
}

char	*ft_substr(char *s, size_t start, size_t len)
{
	size_t	i;
	char	*substr;

	if (s == NULL || ft_strlen(s) <= start || len <= 0)
		return (NULL);
	if (len + start > ft_strlen(s))
		len = ft_strlen(s) - start;
	substr = (char *)malloc(sizeof(char) * (len + 1));
	if (substr == NULL)
		return (NULL);
	i = 0;
	while (i < len && s[start + i] != '\0')
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

char	*copy_to_note(int fd, char *note)
{
	char	*buf;
	int		bytes_read;
	char	*tmp;

	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (buf == NULL)
		return (NULL);
	bytes_read = 1;
	while (bytes_read != 0)
	{
		if (ft_strchr(note, '\n') != NULL)
			break ;
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buf);
			return (NULL);
		}
		buf[bytes_read] = '\0';
		tmp = note;
		note = ft_strjoin(note, buf);
		free(tmp);
	}
	free(buf);
	return (note);
}

char	*get_next_line(int fd)
{
	static char	*note[MAX_FD];
	char		*line;
	char		*tmp;
	size_t		line_len;

	if (fd < 0 || MAX_FD < fd || BUFFER_SIZE <= 0
		|| BUFFER_SIZE >= SIZE_MAX - 1)
		return (NULL);
	note[fd] = copy_to_note(fd, note[fd]);
	if (note[fd] == NULL)
		return (NULL);
	if (ft_strchr(note[fd], '\n') == NULL)
		line_len = ft_strlen(note[fd]);
	else
		line_len = ft_strchr(note[fd], '\n') - note[fd] + 1;
	line = ft_substr(note[fd], 0, line_len);
	tmp = note[fd];
	note[fd] = ft_substr(note[fd], line_len, ft_strlen(note[fd]) - line_len);
	free(tmp);
	return (line);
}
