# include "../includes/get_next_line.h"
# include "../libft/libft.h"
# include "../includes/minishell.h" 
# include "../includes/pipex.h"
# include <readline/readline.h>
# include <readline/history.h>

void minishell(char **envp);

int main(int argc, char **argv, char **envp)
{
	(void)argv;
	if (argc == 1)
		minishell(envp);
	return (0);
}


int	interpret(char *line)
{
	extern char	**environ;
	char		*argv[] = {line, NULL};
	pid_t		pid;
	int			wstatus;

	pid = fork();
	if (pid < 0)
		perror("fork");
	else if (pid == 0)
	{
		// child process
		execute_command(argv, environ);
		perror("execve");
	}
	else
	{
		// parent process
		wait(&wstatus);
		return (WEXITSTATUS(wstatus));
	}
	return (0);
}

void minishell(char **envp)
{
	char *line;
	int  status;

	rl_outstream = stderr;
	status = 0;
	while(1)
	{
		char *line;
		line = readline("minishell$ ");
		if (line == NULL)
		{
			ft_putstr_fd("exit\n", STDERR);
			exit (status);
		}
		if (*line)
			add_history(line);
		// ft_putstr_fd(MINISHELL, STDERR);
		status = interpret(line);
		free(line);
	}
}
