/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_filefd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 20:55:36 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 20:59:24 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

static void	_open_proper_filefd(t_node *node)
{
	if (node->kind == ND_REDIR_OUT)
		node->filefd = open(node->filename->word,
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (node->kind == ND_REDIR_IN)
		node->filefd = open(node->filename->word, O_RDONLY);
	else if (node->kind == ND_REDIR_APPEND)
		node->filefd = open(node->filename->word,
				O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (node->kind == ND_REDIR_HEREDOC)
		node->filefd = open_heredoc(node->delimiter->word);
	else
		fatal_error("get filefd");
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
	else
		_open_proper_filefd(node);
	if (node->filefd < 0)
		fatal_error("open error");
	else if (node->filefd == FAILURE)
		return (FAILURE);
	node->filefd = stash_fd(node->filefd);
	return (get_filefd(node->next));
}