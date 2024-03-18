/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:49:39 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 18:50:49 by yutakagi         ###   ########.fr       */
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
								t_token **tok_list, t_token *cur)
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
		parse_error(cur, &cur);
	*tok_list = cur;
}

// <simple_command> ::= <simple_command_element>
//                    | <simple_command> <simple_command_element>
// <simple_command>は<simple_command_element>を1つ以上持つという意味
t_node	*simple_command(t_token **tok, t_token *cur)
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

// <pipeline> ::= <pipeline> '|' <command> <pipeline>
t_node	*pipeline(t_token **tok, t_token *cur)
{
	t_node	*node;

	node = _the_first_node(ND_PIPELINE);
	node->command = simple_command(&cur, cur);
	if (is_op(cur, "|"))
	{
		node->next = pipeline(&cur, cur->next);
	}
	*tok = cur;
	return (node);
}

t_node	*parse(t_token *tok)
{
	return (pipeline(&tok, tok));
}
