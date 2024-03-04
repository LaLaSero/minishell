# include "../includes/minishell.h"

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

void show_node(t_node *node)
{
	char *node_kind_str[] = {
		"ND_PIPELINE",
		"ND_SIMPLE_CMD",
		"ND_REDIR_OUT",
		"ND_REDIR_IN",
		"ND_REDIR_APPEND",
		"ND_REDIR_HEREDOC",
	};
	if(node == NULL)
		return ;
	printf("__________________________\n");
	printf("node->kind:[%s]\n", node_kind_str[node->kind]);
	if (node->args)
	{
		t_token *tmp = node->args;
		while (tmp)
		{
			printf("args:[%s]\n", tmp->word);
			tmp = tmp->next;
		}
	}
	if (node->filename)
		printf("filename:[%s]\n", node->filename->word);
	if (node->redirect)
		show_node(node->redirect);
	if (node->command)
		show_node(node->command);
	if (node->next)
		show_node(node->next);
	return ;
}