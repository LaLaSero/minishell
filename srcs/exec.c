/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 20:30:09 by kishizu           #+#    #+#             */
/*   Updated: 2024/03/04 20:58:38 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int	isbuiltin(t_node *command)
{
	if (ft_strncmp(command->args->word, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(command->args->word, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(command->args->word, "exit", 5) == 0)
		return (1);
	else if (ft_strncmp(command->args->word, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(command->args->word, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(command->args->word, "unset", 6) == 0)
		return (1);
	return (0);
}

int	exec_builtin(t_node *command)
{
	if (ft_strncmp(command->args->word, "cd", 3) == 0)
		builtin_cd(command);
	else if (ft_strncmp(command->args->word, "echo", 5) == 0)
		builtin_echo(command);
	else if (ft_strncmp(command->args->word, "exit", 5) == 0)
		builtin_exit(1);
	else if (ft_strncmp(command->args->word, "export", 7) == 0)
		builtin_export(command);
	else if (ft_strncmp(command->args->word, "pwd", 4) == 0)
		builtin_pwd();
	else if (ft_strncmp(command->args->word, "unset", 6) == 0)
		builtin_unset();
	return(FAILURE);
}

int	exec(t_node *node)
{
	int	status;

	if (get_filefd(node) == FAILURE)
		return (FAILURE);
	if (node->next == NULL && isbuiltin(node->command))
	{
		status = exec_builtin(node);
	}
	else
	{
		status = exec_pipeline(node);
	}
	return (status);
}
