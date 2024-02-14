/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:37:55 by yutakagi          #+#    #+#             */
/*   Updated: 2024/02/13 23:33:24 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <limits.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stddef.h>
# include <unistd.h>

# ifndef BUFFER_SIZE	
#  define BUFFER_SIZE 42
# endif

# ifndef MAX_FD
#  define MAX_FD 1024
# endif

char	*get_next_line(int fd);
char	*ft_strjoin_gnl(char *s1, char*s2);
char	*ft_strdup_gnl(const char *s);
size_t	ft_strlcpy_gnl(char *dest, char *src, size_t dst_size);
size_t	ft_strlcat_gnl(char *dst, char *src, size_t dstsize);
size_t	ft_strlen_gnl(const char *s);

#endif