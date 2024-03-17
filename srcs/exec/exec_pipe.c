/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:53:02 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/17 20:04:21 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

static void	cpy_pipe(int dst[2], int src[2])
{
	dst[0] = src[0];
	dst[1] = src[1];
}

void	set_pipe(t_node *node)
{
	if (node->next == NULL)
		return ;
	pipe(node->outpipe);
	cpy_pipe(node->next->inpipe, node->outpipe);
}

void set_parent_pipe(t_node *node)
{
	if (node->inpipe[0] != STDIN_FILENO)
		if (close(node->inpipe[0]) < 0)
			fatal_error("close error");
	if (node->next)
		if (close(node->outpipe[1]) < 0)
			fatal_error("close error");
}

void	set_child_pipe(t_node *node)
{
	close(node->outpipe[0]);
	dup2(node->inpipe[0], STDIN_FILENO);
	if (node->inpipe[0] != STDIN_FILENO)
		close(node->inpipe[0]);
	dup2(node->outpipe[1], STDOUT_FILENO);
	if (node->outpipe[1] != STDOUT_FILENO)
		close(node->outpipe[1]);
}