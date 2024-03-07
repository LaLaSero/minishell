# include "../includes/get_next_line.h"
# include "../libft/libft.h"
# include "../includes/minishell.h" 
# include "../includes/pipex.h"

void minishell(char **envp);

t_status status = {};
t_map *envmap;

int main(int argc, char **argv, char **envp)
{
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

void free_token(t_token *tok)
{
	if (tok == NULL)
		return ;
	if (tok->word)
		free(tok->word);
	free_token(tok->next);
	free(tok);
}

void free_node(t_node *node)
{
	if (node == NULL)
		return ;
	free_token(node->args);
	free_token(node->filename);
	free_node(node->redirect);
	free_node(node->next);
	free_node(node->command);
	free(node);
}

void interpret(char *line)
{
	t_token *tok;
	t_node  *node;

	tok = tokenize(line);
	if(status.had_error)
	{
		status.exit_status = ERROR_IN_TOKENIZE;
	}
	else
	{
		node = parse(tok);
		if(status.had_error)
		{
			status.exit_status = ERROR_IN_PARSE;
		}
		else
		{
			// show_node(node);
			// expand(node);
			// status.exit_status = execute(node);
			status.exit_status = exec(node);
		}
	}
	free_node(node);
	free_token(tok);
}

void minishell(char **envp)
{
	int		status;

	rl_outstream = stderr;
	status = 0;
	make_map();
	while(1)
	{
		char *line;
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (ft_strncmp(line, "exit", 4) == 0)
		{
			exit (status);
		}
		if (*line)
			add_history(line);
		if (ft_strncmp(line, "\0", 1) != 0)
			interpret(line);
		free(line);
	}
	exit(status);
}
