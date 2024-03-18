/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 20:30:09 by kishizu           #+#    #+#             */
/*   Updated: 2024/03/18 20:29:44 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

char	**convert_to_argv(t_token *args);
void	dup_redirect(t_node *node);
void	free_argv(char **argv);
void	reset_redirect(t_node *node);

int	exec_builtin(t_node *node)
{
	char	**argv;
	int		status;

	dup_redirect(node->command->redirect);
	argv = convert_to_argv(node->command->args);
	status = 0;
	if (ft_strncmp(node->command->args->word, "cd", 3) == 0)
		status = builtin_cd(argv);
	else if (ft_strncmp(node->command->args->word, "echo", 5) == 0)
		status = builtin_echo(argv);
	else if (ft_strncmp(node->command->args->word, "exit", 5) == 0)
		status = builtin_exit(argv);
	else if (ft_strncmp(node->command->args->word, "export", 7) == 0)
		status = builtin_export(argv);
	else if (ft_strncmp(node->command->args->word, "pwd", 4) == 0)
		status = builtin_pwd();
	else if (ft_strncmp(node->command->args->word, "unset", 6) == 0)
		status = builtin_unset(&argv[1]);
	else if (ft_strncmp(node->command->args->word, "env", 4) == 0)
		status = builtin_env();
	free_argv(argv);
	reset_redirect(node->command->redirect);
	return (status);
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
	else if (node->kind == ND_REDIR_APPEND)
		node->filefd = open(node->filename->word, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (node->kind == ND_REDIR_HEREDOC)
		node->filefd = open_heredoc(node->delimiter->word);
	else
		fatal_error("get filefd");
	if (node->filefd < 0)
		fatal_error("open error");
	else if (node->filefd == FAILURE)
		return (FAILURE);
	node->filefd = stash_fd(node->filefd);
	return (get_filefd(node->next));
}

int	exec_nonbuiltin(t_node *node)
{
	extern char		**environ;
	extern t_status g_status;
	char			**argv;

	dup_redirect(node->command->redirect);
	argv = convert_to_argv(node->command->args);
	execute_command(argv, get_environ(g_status.envmap));
	free_argv(argv);
	reset_redirect(node->command->redirect);
	fatal_error("execve error");
	return (FAILURE);
}

pid_t	exec_pipeline(t_node *node)
{
	pid_t	pid;

	if (node == NULL)
		return (FAILURE);
	set_pipe(node);
	pid = fork();
	if (pid < 0)
	{
		fatal_error("fork error");
		return (FAILURE);
	}
	else if (pid == 0)
	{
		reset_signals();
		set_child_pipe(node);
		if (isbuiltin(node->command) == true)
		{
			exit(exec_builtin(node));
		}
		else
			exit(exec_nonbuiltin(node));
	}
	set_parent_pipe(node);
	if (node->next)
		return (exec_pipeline(node->next));
	return (pid);
}

int	wait_process(pid_t pid)
{
	int		status;
	int		temp_status;
	pid_t	wpid;

	while (1)
	{
		wpid = wait(&temp_status);
		if (wpid == pid)
		{
			if (WIFSIGNALED(temp_status))
				status = WTERMSIG(temp_status) + 128;
			else
				status = WEXITSTATUS(temp_status);
		}
		else if (wpid < 0)
		{
			if (errno == ECHILD)
				break ;
			if (errno == EINTR)
				continue ;
			else
				fatal_error("waitpid error");
			return (FAILURE);
		}
	}
	return (status);
}

int	exec(t_node *node)
{
	int		status;
	pid_t	pid_to_wait;

	if (get_filefd(node) == FAILURE)
	{
		return (FAILURE);
	}
	if (node->next == NULL && isbuiltin(node->command))
	{
		status = exec_builtin(node);
	}
	else
	{
		pid_to_wait = exec_pipeline(node);
		status = wait_process(pid_to_wait);
	}
	return (status);
}
