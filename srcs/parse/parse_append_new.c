/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_append_new.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:11:20 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 19:17:39 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../includes/minishell.h"

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

	node = ft_calloc(1, sizeof(*node));
	if (node == NULL)
		fatal_error("calloc");
	node->kind = kind;
	return (node);
}
