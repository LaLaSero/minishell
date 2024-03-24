/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:56:24 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/24 18:30:45 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

int	is_redir_kind(t_node_kind kind)
{
	if (kind == ND_REDIR_OUT || kind == ND_REDIR_IN
		|| kind == ND_REDIR_APPEND || kind == ND_REDIR_HEREDOC)
		return (true);
	return (false);
}

void	dup_redirect(t_node *node)
{
	if (node == NULL)
		return ;
	if (is_redir_kind(node->kind))
	{
		node->stashed_targetfd = stash_fd(node->targetfd);
		if (dup2(node->filefd, node->targetfd) < 0)
			fatal_error("dup2 error redirect");
	}
	else
	{
		fatal_error("dup_redirect");
	}
	dup_redirect(node->next);
}

void	reset_redirect(t_node *node)
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
			fatal_error("dup2 error reset");
	}
}
