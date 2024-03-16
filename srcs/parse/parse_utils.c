/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 19:11:40 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/16 19:14:21 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../libft/libft.h"
# include "../includes/minishell.h" 

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
