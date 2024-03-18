/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 15:26:53 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 18:51:03 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

t_node	*redirect_out(t_token **tok_loc, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_OUT);
	node->filename = tokdup(tok->next);
	node->targetfd = STDOUT_FILENO;
	*tok_loc = tok->next->next;
	return (node);
}

t_node	*redirect_in(t_token **tok_loc, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_IN);
	node->filename = tokdup(tok->next);
	node->targetfd = STDIN_FILENO;
	*tok_loc = tok->next->next;
	return (node);
}

t_node	*redirect_append(t_token **tok_loc, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_APPEND);
	node->filename = tokdup(tok->next);
	node->targetfd = STDOUT_FILENO;
	*tok_loc = tok->next->next;
	return (node);
}

t_node	*redirect_heredoc(t_token **tok_loc, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_HEREDOC);
	node->delimiter = tokdup(tok->next);
	node->targetfd = STDIN_FILENO;
	*tok_loc = tok->next->next;
	return (node);
}
