/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:49:39 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/14 04:01:25 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../libft/libft.h"
# include "../includes/minishell.h" 
# include "../includes/pipex.h"

// bnf definition by usatie
// <pipeline> = <simple_command> ('|' <pipeline>)
// <simple_command> = <command_element>+
// <command_element> = <word> | <redirection>
// <redirection> = '>' <word>
//               | '<' <word>
//               | '>>' <word>
//               | '<<' <word>
// 
// 以下のbnf定義を元にパーサーを実装する
// <pipeline> ::= <pipeline> '|' <command> <pipeline>
// <command> ::= <simple_command>
//             | <shell_command_word>
//             | <shell_command_word> <redirection_list>
// <simple_command> ::= <simple_command_element>
//                    | <simple_command> <simple_command_element>
// <simple_command_element> ::= <word>
//                            | <redirection>
// <redirection> ::= '>' <word>
//                | '<' <word>
//                | '>>' <word>
//                | '<<' <word>
// source: https://cmdse.github.io/pages/appendix/bash-grammar.html

t_token	*tokdup(t_token *tok);
extern t_status	status;

bool	is_op(t_token *tok, char *op)
{
	if (tok->kind != TK_OP)
		return (false);
	return (strcmp(tok->word, op) == 0);
}

bool	is_control_operator(t_token *tok)
{
	static char	*const	operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
	size_t				i = 0;

	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (strncmp(tok->word, operators[i], strlen(operators[i])) == 0)
			return (true);
		i++;
	}
	return (false);
}

void	append_token(t_token **tok_loc, t_token *tok)
{
	t_token	*last;

	if (*tok_loc == NULL)
	{
		*tok_loc = tok;
		return ;
	}
	last = *tok_loc;
	while (last->next)
		last = last->next;
	last->next = tok;
}

void	append_node(t_node **node_loc, t_node *node)
{
	t_node	*last;

	if (*node_loc == NULL)
	{
		*node_loc = node;
		return ;
	}
	last = *node_loc;
	while (last->next)
		last = last->next;
	last->next = node;
}

t_node	*new_node(t_node_kind kind)
{
	t_node	*node;

	node = calloc(1, sizeof(*node));
	if (node == NULL)
		fatal_error("calloc");
	node->kind = kind;
	return (node);
}

// <simple_command_element> ::= <word> | <redirection>
void	append_command_element(t_node *command, t_token **tok_list, t_token *cur)
{
	extern t_status	t_status;

	if (cur->kind == TK_WORD)
	{
		append_token(&command->args, tokdup(cur));
		cur = cur->next;
	}
	else if (is_op(cur, ">") && cur->next->kind == TK_WORD)
	{
		append_node(&command->redirect, redirect_out(&cur, cur));
	}
	else if (is_op(cur, "<") && cur->next->kind == TK_WORD)
	{
		append_node(&command->redirect, redirect_in(&cur, cur));
	}
	else if (is_op(cur, ">>") && cur->next->kind == TK_WORD)
	{
		append_node(&command->redirect, redirect_append(&cur, cur));
	}
	else if (is_op(cur, "<<") && cur->next->kind == TK_WORD)
	{
		append_node(&command->redirect, redirect_heredoc(&cur, cur));
	}
	else
	{
		parse_error(cur, &cur);
		printf("Unexpected token\n");
	}
	*tok_list = cur;
}

// <simple_command> ::= <simple_command_element>
//                    | <simple_command> <simple_command_element>
// <simple_command>は<simple_command_element>を1つ以上持つという意味
t_node *simple_command(t_token **tok, t_token *cur)
{
	t_node	*node;

	node = new_node(ND_SIMPLE_CMD);
	append_command_element(node, &cur, cur);
	while (cur && cur->kind != TK_EOF && !is_control_operator(cur))
	{
		append_command_element(node, &cur, cur);
	}
	*tok = cur;
	return (node);
}

static t_node	*_the_first_node(t_node_kind kind);

// <pipeline> ::= <pipeline> '|' <command> <pipeline>
t_node *pipeline(t_token **tok, t_token *cur)
{
	t_node *node;

	node = _the_first_node(ND_PIPELINE);
	node->command = simple_command(&cur, cur);
	if (is_op(cur, "|"))
	{
		node->next = pipeline(&cur, cur->next);
	}
	*tok = cur;
	return (node);
}

t_node *parse(t_token *tok)
{
	return (pipeline(&tok, tok));
}

t_token	*tokdup(t_token *tok)
{
	char	*word;

	word = strdup(tok->word);
	if (word == NULL)
		fatal_error("strdup");
	return (new_token(word, tok->kind));
}

static t_node	*_the_first_node(t_node_kind kind)
{
	t_node	*node;

	node = new_node(kind);
	node->inpipe[0] = STDIN_FILENO;
	node->inpipe[1] = -1;
	node->outpipe[0] = -1;
	node->outpipe[1] = STDOUT_FILENO;
	return (node);
}
