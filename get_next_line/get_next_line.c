/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:37:53 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/18 18:08:42 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/get_next_line.h"

char	*ft_strchr_gnl(const char *s, int c)
{
	size_t	i;
	char	*p;

	i = 0;
	if (s == NULL)
		return (NULL);
	p = (char *)s;
	if (c == '\0')
		return (p + ft_strlen_gnl(p));
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

	if (s == NULL || ft_strlen_gnl(s) <= start || len <= 0)
		return (NULL);
	if (len + start > ft_strlen_gnl(s))
		len = ft_strlen_gnl(s) - start;
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
		if (ft_strchr_gnl(note, '\n') != NULL)
			break ;
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buf);
			return (NULL);
		}
		buf[bytes_read] = '\0';
		tmp = note;
		note = ft_strjoin_gnl(note, buf);
		free(tmp);
	}
	free(buf);
	return (note);
}

char	*get_next_line(int fd)
{
	static char	*note;
	char		*line;
	char		*tmp;
	size_t		line_len;

	if (fd < 0 || MAX_FD < fd || BUFFER_SIZE <= 0
		|| BUFFER_SIZE >= SIZE_MAX - 1)
		return (NULL);
	note = copy_to_note(fd, note);
	if (note == NULL)
		return (NULL);
	if (ft_strchr_gnl(note, '\n') == NULL)
		line_len = ft_strlen_gnl(note);
	else
		line_len = ft_strchr_gnl(note, '\n') - note + 1;
	line = ft_substr(note, 0, line_len);
	tmp = note;
	note = ft_substr(note, line_len, ft_strlen_gnl(note) - line_len);
	free(tmp);
	return (line);
}

// #include <stdio.h>
// int	main()
// {
// 	int fd = open("42_with_nl", O_RDONLY);
// 	char *line;
// 	int i = 0;
// 	while (i < 5)
// 	{
// 		line = get_next_line(fd);
// 		printf("%s",line);
// 		free(line);
// 		i++;
// 	}
// 	close(fd);
// }
// __attribute__((destructor))
// static void destructor() {
// 	system("leaks -q a.out");
// }