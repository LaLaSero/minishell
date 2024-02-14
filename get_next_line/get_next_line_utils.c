/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:37:57 by yutakagi          #+#    #+#             */
/*   Updated: 2024/02/13 23:37:19 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/get_next_line.h"

size_t	ft_strlen_gnl(const char *s)
{
	size_t	i;

	if (s == NULL)
		return (0);
	i = 0;
	while (*s != '\0')
	{
		i++;
		s++;
	}
	return (i);
}

size_t	ft_strlcat_gnl(char *dst, char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	k = 0;
	if (dst == NULL && dstsize == 0)
		return (0);
	while (dst[i] != '\0')
		i++;
	while (src[j] != '\0')
		j++;
	if (dstsize <= i)
		return (j + dstsize);
	j += i;
	while (src[k] != '\0' && i < dstsize - 1)
	{
		dst[i] = src[k];
		i++;
		k++;
	}
	dst[i] = '\0';
	return (j);
}

size_t	ft_strlcpy_gnl(char *dest, char *src, size_t dst_size)
{
	size_t	i;

	if (dest == NULL && dst_size == 0)
		return (0);
	i = 0;
	while (dst_size != 0 && src[i] && i < dst_size - 1)
	{
		dest[i] = src[i];
		i++;
	}
	if (dst_size != 0)
		dest[i] = '\0';
	return (ft_strlen_gnl(src));
}

char	*ft_strdup_gnl(const char *s)
{
	char	*str;

	str = (char *)malloc(sizeof(char) * (ft_strlen_gnl(s) + 1));
	if (str == NULL)
		return (NULL);
	ft_strlcpy_gnl(str, (char *)s, ft_strlen_gnl(s) + 1);
	return (str);
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*str;

	if (s1 == NULL && s2 != NULL)
		return (ft_strdup_gnl(s2));
	if (s1 != NULL && s2 == NULL)
		return (ft_strdup_gnl(s1));
	if (s1 == NULL && s2 == NULL)
		return (NULL);
	str = (char *)malloc(sizeof(char) * (ft_strlen_gnl(s1) + ft_strlen_gnl(s2) + 1));
	if (str == NULL)
		return (NULL);
	ft_strlcpy_gnl(str, (char *)s1, ft_strlen_gnl(s1) + 1);
	ft_strlcat_gnl(str, (char *)s2, ft_strlen_gnl(s1) + ft_strlen_gnl(s2) + 1);
	return (str);
}
