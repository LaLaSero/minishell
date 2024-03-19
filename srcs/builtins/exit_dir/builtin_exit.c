/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:59:58 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/19 14:15:47 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 
#include "../libft/libft.h"

int	builtin_exit(char **argv, int status)
{
	// extern t_status	g_status;
	bool			is_valud;
	long			res;

	ft_putstr_fd("exit\n", STDOUT);
	is_valud = true;
	if (!argv[1])
		exit(status);
	else if (argv[2])
	{
		print_exit_error(NULL, "too many arguments\n");
		return (FAILURE);
	}
	else
	{
		res = ft_atol(argv[1], &is_valud);
		if (is_valud == true)
		{
			exit(fit_res_to_int(res));
		}
		print_exit_error(argv[1], ": numeric argument required\n");
		exit(255);
	}
}
