# include "../includes/get_next_line.h"
# include "../libft/libft.h"
# include "../includes/minishell.h" 
# include "../includes/pipex.h"
# include <readline/readline.h>
# include <readline/history.h>

void minishell(char **envp);

int token_test(t_token *tok)
{
	while (tok)
{
    char *kind_str;
    switch (tok->kind) {
        case TK_WORD:
            kind_str = "WORD";
            break;
        case TK_RESERVED:
            kind_str = "RESERVED";
            break;
        case TK_OP:
            kind_str = "OP";
            break;
        case TK_EOF:
            kind_str = "EOF";
            break;
        default:
            kind_str = "UNKNOWN";
    }
    printf("[%s] [%s]\n", kind_str, tok->word ? tok->word : "NULL");
    tok = tok->next;

	}
	return 0;
}

int node_test(t_node *node)
{
	printf("node_test\n");
	if (node == NULL || node->command->args == NULL)
	{
		printf("node is NULL\n");
		return 0;
	}
	// if (node->next == NULL || node->next->command->args == NULL)
	// {
	// 	printf("node->next is NULL\n");
	// 	return 0;
	// }
	t_token *tmp = node->command->args;
	while (tmp)
	{
		printf("command->args:[%s]\n", tmp->word);
		tmp = tmp->next;
	}
	printf("command->kind:[%d]\n", node->command->kind);
	printf("command->redirect->filename:[%s]\n", node->command->redirect->filename->word);
	printf("command->redirect->target_fd:[%d]\n", node->command->redirect->targetfd);
	printf("node->next->command->args:[%s]\n", node->next->command->args->word);
	printf("node->next->command->args->next->word:[%s]\n", node->next->command->args->next->word);		
	printf("node->next->command->kind:[%d]\n", node->next->command->kind);
	return 0;
}

int main(int argc, char **argv, char **envp)
{
	// return (token_test(tokenize("echo    'hello world' world")));
	(void)argv;
	if (argc == 1)
		minishell(envp);
	return (0);
}

void handle_error(char *msg)
{
	perror(msg);
	exit(-1);
}

void excecute_token(t_token *tok)
{
	extern char	**environ;
	char **command_splitted;
	int token_count = 0;

	t_token *tmp = tok;
	while (tmp)
	{
		token_count++;
		tmp = tmp->next;
	}
	command_splitted = malloc(sizeof(char *) * (token_count + 1));
	if (command_splitted == NULL)
		handle_error("malloc");
	for (int i = 0; i < token_count; i++)
	{
		command_splitted[i] = tok->word;
		tok = tok->next;
	}
	command_splitted[token_count] = NULL;
	execute_command(command_splitted, environ);
	exit(1);
}

int	interpret(char *line)
{
	pid_t		pid;
	int			wstatus;
	t_token *tok;
	t_node  *node;

	pid = fork();
	if (pid < 0)
		handle_error("fork");
	else if (pid == 0)
	{
		// child process
		tok = tokenize(line);
		token_test(tok);
		expand_token(tok);
		node = parse(tok);
		printf("parse done\n");
		node_test(node);
		// free_token(tok);
		// free_node(node);
		// expand_token(tok);
		// expand_token(node);
		// token_test(tok);
		// excecute_token(tok);
		// execute_token(node);
		// 構文= 解析(tok);
		// 実行(構文);
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
	char	*line;
	int		status;

	rl_outstream = stderr;
	status = 0;
	while(1)
	{
		char *line;
		line = readline("minishell$ ");
		if (line == NULL || ft_strncmp(line, "exit", 4) == 0)
		{
			exit (status);
		}
		if (*line)
			add_history(line);
		status = interpret(line);
		free(line);
	}
}
