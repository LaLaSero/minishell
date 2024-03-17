/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 10:40:38 by marvin            #+#    #+#             */
/*   Updated: 2024/03/18 01:37:17 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*p;
	unsigned char	*q;

	if (!s1 || !s2)
		return (0);
	p = (unsigned char *) s1;
	q = (unsigned char *) s2;
	i = 0;
	while (i < n && p[i] == q[i] && p[i] != '\0' && q[i] != '\0')
		i++;
	if (i == n || p[i] == q[i])
		return (0);
	else
		return (p[i] - q[i]);
}

// #include <string.h>
// #include <stdio.h>
// int main()
// {
// 	const char s1[] = "ABCD";
// 	const char s2[] = "ABCA";
// 	size_t i = 4;
// 	printf("%d\n",ft_strncmp(s1,s2,i));
// 	printf("%d\n",strncmp(s1,s2,i));
// }