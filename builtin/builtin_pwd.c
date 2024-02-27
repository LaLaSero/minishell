/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 17:55:10 by kishizu           #+#    #+#             */
/*   Updated: 2024/02/20 21:46:45 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	builtin_pwd(void)
{
	char	path[ARG_MAX];

	ft_bzero(path, ARG_MAX);
	getcwd(path, ARG_MAX);
	ft_putendl_fd (path, STDOUT_FILENO);
}
