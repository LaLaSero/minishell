/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 20:30:09 by kishizu           #+#    #+#             */
/*   Updated: 2024/03/13 17:11:17 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

char **convert_to_argv(t_token *args);
void dup_redirect(t_node *node);
void free_argv(char **argv);
void reset_redirect(t_node *node);

int	isbuiltin(t_node *command)
{
	// printf("isbuiltin\n");
	if (ft_strncmp(command->args->word, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(command->args->word, "echo", 5) == 0)
		return (1);
// 	else if (ft_strncmp(command->args->word, "exit", 5) == 0)
// 		return (1);
	else if (ft_strncmp(command->args->word, "export", 7) == 0)
		return (true);
	else if (ft_strncmp(command->args->word, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(command->args->word, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(command->args->word, "env", 4) == 0)
		return (true);
	return (false);
}

int	exec_builtin(t_node *node)
{
	char **argv;

	dup_redirect(node->command->redirect);
	argv = convert_to_argv(node->command->args);
	if (ft_strncmp(node->command->args->word, "cd", 3) == 0)
		builtin_cd(argv);
	else if (ft_strncmp(node->command->args->word, "echo", 5) == 0)
		builtin_echo(argv);
	// else if (ft_strncmp(command->args->word, "exit", 5) == 0)
	// 	builtin_exit(1);
	else if (ft_strncmp(node->command->args->word, "export", 7) == 0)
		builtin_export(argv);
	else if (ft_strncmp(node->command->args->word, "pwd", 4) == 0)
		builtin_pwd();
	else if (ft_strncmp(node->command->args->word, "unset", 6) == 0)
		builtin_unset(&argv[1]);
	else if (ft_strncmp(node->command->args->word, "env", 4) == 0)
		builtin_env();
	free_argv(argv);
	reset_redirect(node->command->redirect);
	return (FAILURE);
}

int stash_fd(int fd)
{
	int	stash;

	stash = fcntl(fd, F_DUPFD, 10);
	if (stash < 0)
	{
		fatal_error("stash error");
		return (FAILURE);
	}
	if (close(fd) < 0)
	{
		fatal_error("close error");
		return (FAILURE);
	}
	return (stash);

}

int open_heredoc(char *delimiter)
{
	char *line;
	int	pipefd[2];

	if (pipe(pipefd) < 0)
	{
		fatal_error("pipe error");
		return (FAILURE);
	}
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			close(pipefd[1]);
			close(pipefd[0]);
			return (FAILURE);
		}
		if (strncmp(line, delimiter, strlen(delimiter)) == 0)
		{
			free(line);
			break;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	return (SUCCESS);
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
		node ->filefd = open_heredoc(node->delimiter->word);
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
	node->filefd = stash_fd(node->filefd);
	return (get_filefd(node->next));
}

int is_redir_kind(t_node_kind kind)
{
	if (kind == ND_REDIR_OUT || kind == ND_REDIR_IN
		|| kind == ND_REDIR_APPEND || kind == ND_REDIR_HEREDOC)
		return (true);
	return (false);
}

void dup_redirect(t_node *node)
{
	if (node == NULL)
		return ;
	if (is_redir_kind(node->kind))
	{
		if (dup2(node->filefd, node->targetfd) < 0)
			fatal_error("dup2 error");
	}
	else
	{
		fatal_error("dup_redirect");
	}
	dup_redirect(node->next);
}

int get_sizeof_token(t_token *args)
{
	int		i;
	t_token	*tmp;

	i = 0;
	tmp = args;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char **convert_to_argv(t_token *args)
{
	int		i;
	char	**argv;
	t_token	*tmp;

	i = get_sizeof_token(args);
	argv = calloc(i + 1, sizeof(*argv));
	if (argv == NULL)
		fatal_error("calloc");
	i = 0;
	tmp = args;
	while (tmp)
	{
		argv[i] = strdup(tmp->word);
		tmp = tmp->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

void free_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void reset_redirect(t_node *node)
{
	if (node == NULL)
		return ;
	reset_redirect(node->next);
	if (is_redir_kind(node->kind))
	{
		if (close(node->filefd) < 0)
			fatal_error("close error");
		if (close(node->targetfd) < 0)
			fatal_error("close error");
		if (dup2(node->stashed_targetfd, node->targetfd) < 0)
			fatal_error("dup2 error");
	}
}

extern t_map *envmap;

// void show_argv(char **argv)
// {
// 	int	i;

// 	i = 0;
// 	while (argv[i])
// 	{
// 		printf("argv[%d]: %s\n", i, argv[i]);
// 		i++;
// 	}
// }

// void show_envp(char **envp)
// {
// 	int	i;

// 	i = 0;
// 	while (envp[i])
// 	{
// 		printf("envp[%d]: %s\n", i, envp[i]);
// 		i++;
// 	}
// }

int exec_nonbuiltin(t_node *node)
{
	char *path;
	char **argv;
	extern char **environ;

	dup_redirect(node->command->redirect);
	argv = convert_to_argv(node->command->args);
	execute_command(argv, get_environ(envmap));
	free_argv(argv);
	reset_redirect(node->command->redirect);
	fatal_error("execve error");
}

static void	cpy_pipe(int dst[2], int src[2])
{
	dst[0] = src[0];
	dst[1] = src[1];
}

void	set_pipe(t_node *node)
{
	if (node->next == NULL)
		return ;
	pipe(node->outpipe);
	cpy_pipe(node->next->inpipe, node->outpipe);
}

void reset_parent_pipe(t_node *node)
{
	if (node->inpipe[0] != STDIN_FILENO)
		if (close(node->inpipe[0]) < 0)
			fatal_error("close error");
	if (node->next)
		if (close(node->outpipe[1]) < 0)
			fatal_error("close error");
}

void set_child_pipe(t_node *node)
{
	close(node->outpipe[0]);
	dup2(node->inpipe[0], STDIN_FILENO);
	if (node->inpipe[0] != STDIN_FILENO)
		close(node->inpipe[0]);
	dup2(node->outpipe[1], STDOUT_FILENO);
	if (node->outpipe[1] != STDOUT_FILENO)
		close(node->outpipe[1]);
}

pid_t exec_pipeline(t_node *node)
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
		set_child_pipe(node);
		exit(exec_nonbuiltin(node));
		// if (isbuiltin(node) == true)
		// {
		// 	printf("isbuiltindesu\n");
		// 	exit(exec_builtin(node));
		// }
		// else
		// 	exit(exec_nonbuiltin(node));
	}
	wait(NULL);
	reset_parent_pipe(node);
	if (node->next)
		return (exec_pipeline(node->next));
	return (pid);
}

int wait_process(pid_t pid)
{
	int	status;

	while (1)
	{
		
		if (waitpid(pid, &status, 0) < 0)
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
	int	status;
	pid_t pid_to_wait;

	if (get_filefd(node) == FAILURE)
		return (FAILURE);
	// exec_pipeline(node);
	// return 0;
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
