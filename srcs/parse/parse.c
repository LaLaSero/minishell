/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:49:39 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/23 16:16:40 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../includes/minishell.h"

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
// <simple_command_element> ::= <word> | <redirection>
void	append_command_element(t_node *command,
								t_token **tok_list, t_token *cur, int *error)
{
	if (cur->kind == TK_WORD)
	{
		append_token(&command->args, tokdup(cur));
		cur = cur->next;
	}
	else if (is_op(cur, ">") && cur->next->kind == TK_WORD)
		append_node(&command->redirect, redirect_out(&cur, cur));
	else if (is_op(cur, "<") && cur->next->kind == TK_WORD)
		append_node(&command->redirect, redirect_in(&cur, cur));
	else if (is_op(cur, ">>") && cur->next->kind == TK_WORD)
		append_node(&command->redirect, redirect_append(&cur, cur));
	else if (is_op(cur, "<<") && cur->next->kind == TK_WORD)
		append_node(&command->redirect, redirect_heredoc(&cur, cur));
	else
	{
		parse_error(cur, &cur);
		*error = true;
	}
	*tok_list = cur;
}

// <simple_command> ::= <simple_command_element>
//                    | <simple_command> <simple_command_element>
// <simple_command>は<simple_command_element>を1つ以上持つという意味
t_node	*simple_command(t_token **tok, t_token *cur, int *error)
{
	t_node	*node;

	node = new_node(ND_SIMPLE_CMD);
	append_command_element(node, &cur, cur, error);
	while (cur && cur->kind != TK_EOF && !is_control_operator(cur))
	{
		append_command_element(node, &cur, cur, error);
	}
	*tok = cur;
	return (node);
}

// <pipeline> ::= <pipeline> '|' <command> <pipeline>
t_node	*pipeline(t_token **tok, t_token *cur, int *error)
{
	t_node	*node;

	node = _the_first_node(ND_PIPELINE);
	node->command = simple_command(&cur, cur, error);
	if (is_op(cur, "|") && cur->next->kind != TK_EOF)
	{
		node->next = pipeline(&cur, cur->next, error);
	}
	*tok = cur;
	return (node);
}

t_node	*parse(t_token *tok, int *error)
{
	return (pipeline(&tok, tok, error));
}
