/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 20:30:09 by kishizu           #+#    #+#             */
/*   Updated: 2024/03/04 21:51:49 by yutakagi         ###   ########.fr       */
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
	return (FAILURE);
}

int	get_filefd(t_node *node)
{
	if (node == NULL)
		return (SUCCESS);
	if (node->kind == ND_PIPELINE)
	{
		if (get_filefd(node->command) == EXIT_FAILURE)
			return (FAILURE);
		if (get_filefd(node->next) == FAILURE)
			return (FAILURE);
		return (SUCCESS);
	}
	else if (node->kind == ND_SIMPLE_CMD)
		return (get_filefd(node->redirect));
	else if (node->kind == ND_REDIR_OUT)
		node->filefd = open(node->filename->word, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (node->kind == ND_REDIR_IN)
		node->filefd = open(node->filename->word, O_RDONLY);
	else if (node->kind == ND_REDIR_HEREDOC)
		node ->filefd = open_heredoc();
	else
	{
		fatal_error("get filefd");
		return (FAILURE);
	}
	if (node->filefd < 0)
	{
		fatal_error("open error");
		return (FAILURE);
	}
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
