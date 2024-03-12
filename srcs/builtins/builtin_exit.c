/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:59:58 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/10 19:18:11 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h" 
# include "../libft/libft.h"

bool is_only_digit(char *str)
{
	int i = 0;

	while (str[i])
	{
		if (ft_isdigit(str[i]) == false)
			return (false);
		i++;
	}
	return (true);
}

int builtin_exit(char **argv)
{
	extern t_status status;

	if (argv[1] == NULL)
		exit(status.exit_status);
	else if (argv[2] != NULL)
	{
		ft_putstr_fd("exit\n", STDERR);
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR);
		status.exit_status = 1;
	}
	else
	{
		if (is_only_digit(argv[1]) == false)
		{
			status.exit_status = ft_atol(argv[1]);
		}
		else
		{
			status.exit_status = 255;
		}
	}
}