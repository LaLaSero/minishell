/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 21:24:17 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/26 00:24:04 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_token(t_token *tok)
{
	if (tok == NULL)
		return ;
	if (tok->word)
		free(tok->word);
	free_token(tok->next);
	free(tok);
}

void	free_node(t_node *node)
{
	if (node == NULL)
		return ;
	free_token(node->args);
	free_token(node->filename);
	free_token(node->delimiter);
	free_node(node->redirect);
	free_node(node->next);
	free_node(node->command);
	free(node);
}

void	free_argv_str(char **argv, char *str)
{
	free_argv(argv);
	free(str);
}
