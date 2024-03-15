/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:59:58 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/16 01:53:22 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h" 
# include "../libft/libft.h"

bool	does_overflow(long result, int flag, char *string, bool *is_valid)
{
	if (result * flag > LONG_MAX / 10
		|| (result * flag == LONG_MAX / 10 && *string - '0' > 7))
		{
			*is_valid = false;
			return (true);
		}
	if (result * flag < LONG_MIN / 10
		|| (result * flag == LONG_MIN / 10 && *string - '0' > 8))
		{
			*is_valid = false;
			return (true);
		}
	return (false);
}

long	ft_atol(char *string, bool *is_valid)
{
	int		flag;
	long	result;

	while (*string == ' ' || *string == '\t' || *string == '\n'
		|| *string == '\v' || *string == '\f' || *string == '\r')
		string++;
	flag = 1;
	if (*string == '-' || *string == '+')
	{
		if (*string == '-')
			flag *= -1;
		string++;
	}
	result = 0;
	while (*string >= '0' && *string <= '9')
	{
		if (does_overflow(result, flag, string, is_valid) == true)
			return (FAILURE);
		result = result * 10 + (*string++ - '0');
	}
	if (*string != '\0')
		*is_valid = false;
	return (result * flag);
}

bool is_only_digit(char *str)
{
	int i = 0;

	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == false)
			return (false);
		i++;
	}
	return (true);
}

int	fit_res_to_int(long res)
{
	res = res % 256;
	if (res < 0)
		res += 256;
	return ((int)res);
}

void	print_exit_error(char *arg, char *message)
{
	ft_putstr_fd("exit\n", STDERR);
	ft_putstr_fd("minishell: exit: ", STDERR);
	if (arg)
		ft_putstr_fd(arg, STDERR);
	ft_putstr_fd(message, STDERR);
}

int builtin_exit(char **argv)
{
	extern t_status	status;
	bool			is_valud;
	long			res;

	is_valud = true;
	if (!argv[1])
		exit(status.exit_status);
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
