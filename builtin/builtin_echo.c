/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 18:03:50 by kishizu           #+#    #+#             */
/*   Updated: 2024/02/20 21:45:10 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"


void	builtin_echo(char *line)
{
	//そのまま出力
		ft_putendl_fd(line, STDOUT_FILENO);
	//-n optionは最後に改行をつけない
		ft_putstr_fd(line, STDOUT_FILENO);
}