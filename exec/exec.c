/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:23:38 by kishizu           #+#    #+#             */
/*   Updated: 2024/02/21 21:53:25 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	isbuiltin(char *cmd)
{
	if (ft_strncmp(cmd, "cd", 3))
		return (1);
	else if (ft_strncmp(cmd, "echo", 5))
		return (1);
	else if (ft_strncmp(cmd, "exit", 5))
		return (1);
	else if (ft_strncmp(cmd, "export", 7))
		return (1);
	else if (ft_strncmp(cmd, "pwd", 4))
		return (1);
	else if (ft_strncmp(cmd, "unset", 6))
		return (1);
	return (0);
}

char	**set_cmd()
{

}

void	exec_nonbuiltin(cmd)
{
	execve(cmd, cmd, NULL);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	ft_exec(char *cmd)
{
	if (isbuiltin(cmd))
		exec_builtin(cmd);
	else
		exec_nonbuiltin(cmd);
}
